// #include <filesystem>
#include "BlockSystem/block.h"
#include "BlockSystem/checkboxBlock.h"
#include "BlockSystem/pageBlock.h"
#include "BlockSystem/toggleBlock.h"
#include "BlockSystem/bulletBlock.h"
#include "PageManager.h"
#include <PageModel.h>

#include <functional>
#include <qabstractitemmodel.h>
#include <qiodevicebase.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpointer.h>
#include <qstringview.h>
#include <qvariant.h>
#include <QDebug>

int PageModel::rowCount(const QModelIndex &parent) const{
	return blockList.count();
}
QVariant PageModel::data(const QModelIndex &index, int role) const{
	if(index.row()<0 || index.row()>rowCount()-1) return QVariant();
	switch(role){
		case LogicBlockRole:{
				QObject* obj = blockList.at(index.row());
				return QVariant::fromValue(obj);
			}
		default:
			return QVariant();
	}
}

QHash<int,QByteArray> PageModel::roleNames() const {
	QHash<int,QByteArray> roles;
	roles[LogicBlockRole] = "logicobject";
	return roles;
}
bool PageModel::setData(const QModelIndex &index, const QVariant &value, const int role) {
	if(index.isValid()&&role==Qt::EditRole){
		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

Qt::ItemFlags PageModel::flags(const QModelIndex &index)const{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PageModel::append(Block *b){
	int rc = rowCount();
	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(b);
	realList.append(b);
	this->endInsertRows();
	return true;
} 

bool PageModel::append(){
	int rc = rowCount();
	Block* b =  new TextBlock(this);
	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(b);
	realList.append(b);
	this->endInsertRows();
	return true;
} 
bool PageModel::append(QVariant blockType){
	int rc = rowCount();
	Block *b;
	if (blockType.toString()=="checkboxBlock"){
		b = new CheckboxBlock(this);
	}else if(blockType.toString()=="bulletBlock"){
		b = new BulletBlock(this);
	}else if(blockType.toString()=="pageBlock" || blockType.toString()=="databaseBlock"){
		int id=1;
		QString type = (blockType.toString() == "databaseBlock") ? "DataBase" : "note";
		if(callback){
			id = callback(getPageData()->id, type);
			qDebug()<<" id : "<<id;
		}
		PageBlock *pb = new PageBlock(this);
		pb->setPageId(id);
		b = pb;
	}
	else if(blockType.toString()=="toggleBlock"){
		b = new ToggleBlock(this);
	}
	else{
		b = new TextBlock(this);
	}
	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(b);
	realList.append(b);
	this->endInsertRows();
	return true;
} 
bool PageModel::insert(QVariant blockType, const int _index){
	int rc = _index;
	Block *b;
	if (blockType.toString()=="checkboxBlock"){
		b = new CheckboxBlock(this);
	}else if(blockType.toString()=="bulletBlock"){
		b = new BulletBlock(this);
	}else if(blockType.toString()=="toggleBlock"){
		b = new ToggleBlock(this);
	}else if(blockType.toString()=="pageBlock"){
		PageBlock *pb = new PageBlock(this);
		b = pb;
	}else{
		b = new TextBlock(this);
	}

	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.insert(rc,b);
	realList.append(b);
	this->endInsertRows();
	return true;
}
void PageModel::removeRow(const int index){
	int ir = index;
	if(ir>=rowCount() ||ir<0){
		return;
	}
	Block* b = blockList.at(index);
	if(b->parentBlock()){
		b->parentBlock()->children.removeOne(b);
	}
	rmChildList(b);
	this->beginRemoveRows(QModelIndex(),ir,ir);
	blockList.removeAt(ir);
	realList.removeOne(b);
	this->endRemoveRows();
	delete b;
}

QByteArray PageModel::listToJson(){
    QJsonObject root;
    if (pageData && !pageData->content.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(pageData->content);
        if (doc.isObject()) root = doc.object();
    }

	QJsonArray array;
	for(auto item : realList){
		if(!item->parentBlock())
			array.append(item->blockToJson());
	}
	root["blockList"]=array;

	QJsonDocument doc(root);
	QByteArray res = doc.toJson(QJsonDocument::Compact);
	return res;
}

void PageModel::parseJson(QByteArray input){
	QJsonDocument doc = QJsonDocument::fromJson(input);
	QJsonArray array;
	if(doc["blockList"].isArray())
		array= doc["blockList"].toArray();

	int r = rowCount();
	if(r > 0){
		this->beginRemoveRows(QModelIndex(),0,r-1);
		for(auto b : realList) delete b;
		blockList.clear();
		realList.clear();
		this->endRemoveRows();
	}

	std::function<void(const QJsonArray&, Block*)> loadBlocks = [&](const QJsonArray& arr, Block* parent) {
		for(auto item : arr){
			auto obj = item.toObject();
			if(!obj.contains("type")) continue;
			QString blockType = obj["type"].toString();
			QJsonObject content = obj["content"].toObject();
			
			Block *b = nullptr;
			if(blockType=="textBlock"){
				TextBlock *tb = new TextBlock(this);
				tb->setText(content["text"].toString());
				b=tb;
			}else if(blockType=="checkboxBlock"){
				CheckboxBlock *cb = new CheckboxBlock(this);
				cb->setText(content["text"].toString());
				cb->setState(content["state"].toVariant());
				b=cb;
			}else if(blockType=="bulletBlock"){
				BulletBlock *bb = new BulletBlock(this);
				bb->setText(content["text"].toString());
				b=bb;
			}else if(blockType=="pageBlock"){
				PageBlock * pb = new PageBlock(this);
				pb->setPageId(content["id"].toInt());
				b=pb;
			}else if(blockType=="toggleBlock"){
				ToggleBlock *tg = new ToggleBlock(this);
				tg->setText(content["text"].toString());
				tg->setExpanded(content["expanded"].toBool());
				b=tg;
			}

			if(b){
				realList.append(b);
				b->setParentBlock(parent);
				if(parent) parent->children.append(b);

				bool visible = true;
				Block* p = parent;
				while(p){
					auto tb = qobject_cast<ToggleBlock*>(p);
					if(tb && !tb->expanded()){
						visible = false;
						break;
					}
					p = p->parentBlock();
				}

				if(visible){
					int rc = rowCount();
					beginInsertRows(QModelIndex(), rc, rc);
					blockList.append(b);
					endInsertRows();
				}

				if(obj.contains("children")){
					loadBlocks(obj["children"].toArray(), b);
				}
			}
		}
	};

	loadBlocks(array, nullptr);
}

void PageModel::parseJson(QString input){
	PageModel::parseJson(input.toUtf8());
}

QVariant PageModel::getLogic(const int _index){
	QModelIndex in = index(_index,0);
	return data(in,LogicBlockRole);
}

PageData* PageModel::getPageData(){
	if(pageData){
		return pageData;
	}else{
		PageData *pd = new PageData{-1,-1,"",""};
		return pd;
	}
}

void PageModel::setPageData(PageData* pd){
	pageData = pd;
}

void PageModel::insertChildList(ToggleBlock* toggle){
	int toggleIndex = blockList.indexOf(toggle);
	if(toggleIndex <0) return;
	QVector<Block*> visibleChildren;

	std::function<void(Block*)> walk;

	walk = [&](Block* block){
		visibleChildren.append(block);
		auto tb = qobject_cast<ToggleBlock*>(block);
		if(!tb || tb->expanded()){
			for(auto child : block->children)
				walk(child);
		}
	};
	for(auto child: toggle->children){
		walk(child);
	}
	if(visibleChildren.isEmpty()) return;

	int first = toggleIndex+1;
	int last = first + visibleChildren.size()-1;

	beginInsertRows({},first,last);
	for(int i=0; i<visibleChildren.size();i++)
		blockList.insert(first+i, visibleChildren[i]);

	endInsertRows();
}
void PageModel::rmChildList(Block* toggle){
	int toggleIndex = blockList.indexOf(toggle);
	if(toggleIndex<0) return;
	int count = countChilds(toggle);

	if(count==0 || count > blockList.count())return;

	beginRemoveRows({},toggleIndex+1, toggleIndex+count);
	for(int i=0;i<count; i++)
		blockList.removeAt(toggleIndex+1);
	endRemoveRows();
}

void PageModel::toggle(ToggleBlock* toggle){
	if(toggle->expanded()){
		rmChildList(toggle);
		toggle->setExpanded(false);
	}
	else{
		toggle->setExpanded(true);
		insertChildList(toggle);
	}
}

int PageModel::countChilds(Block* block){
	int counter = 0;
	if(block->children.isEmpty()==false){
		auto tb = qobject_cast<ToggleBlock*>(block);
		if(!tb || tb->expanded()){
			for(auto b: block->children){
				counter++;
				counter += countChilds(b);
			}
		}
	}
	return counter;
}
void PageModel::updateChildIndent(Block* block){
	int counter = countChilds(block);
	if(block->children.isEmpty()==false){
		for(auto b: block->children){
			b->levelChanged();
			if(counter>0){
				if(b->children.isEmpty()==false)updateChildIndent(b);
				counter--;
			}
		}
	}
}
void PageModel::addIndentBlock(Block* block){
	int row = blockList.indexOf(block);
	if(row <=0) return;
	Block* prev=blockList[row-1];

	Block* parent = prev;

	if(parent && block->parentBlock()!=parent){
		parent->children.append(block);
		if(block->parentBlock()) block->parentBlock()->children.removeOne(block);
		block->setParentBlock(parent);
		emit block->levelChanged();
		updateChildIndent(block);
		
		auto tb = qobject_cast<ToggleBlock*>(parent);
		if(tb && tb->expanded()==false){
			rmChildList(block);
			this->beginRemoveRows(QModelIndex(),row,row);
			blockList.removeAt(row);
			this->endRemoveRows();
		}
		qDebug()<<"block indented under" << parent->typeName();
	}
}
void PageModel::rmIndentBlock(Block* block){
	Block* p = block->parentBlock();
	if(p==nullptr) return;
	Block* pp = p->parentBlock();

	if(p && pp){
		int index = p->children.indexOf(block);
		if(index>=0){
			p->children.removeOne(block);
			pp->children.append(block);
			block->setParentBlock( pp );
		}
	}else{
		block->setParentBlock(nullptr);
		p->children.removeOne(block);
	}
	emit block->levelChanged();
	updateChildIndent(block);
}
