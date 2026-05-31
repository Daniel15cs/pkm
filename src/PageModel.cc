// #include <filesystem>
#include "BlockSystem/checkboxBlock.h"
#include "BlockSystem/pageBlock.h"
#include "PageManager.h"
#include <PageModel.h>

#include <qabstractitemmodel.h>
#include <qiodevicebase.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpointer.h>
#include <qstringview.h>
#include <qvariant.h>
#include <QDebug>
int PageModel::rowCount(const QModelIndex &parent) const{
	return blockList.count();
}
QVariant PageModel::data(const QModelIndex &index, int role) const{
    if(index.row()<0 || index.row()>rowCount()) return QVariant();
	switch(role){
		case LogicBlockRole:{
			QObject* obj = blockList.at(index.row());
			return QVariant::fromValue(obj);
		}
		// case TextBlockRole :{
		// 	return blockList.at(index.row())->getData().toString();
		// }
		default:
			return QVariant();
	}
}
// QVariant MyListModel::displayData(const QModelIndex &index){
// }

QHash<int,QByteArray> PageModel::roleNames() const {
	QHash<int,QByteArray> roles;
	roles[LogicBlockRole] = "logicobject";
	return roles;
}
bool PageModel::setData(const QModelIndex &index, const QVariant &value, const int role) {
	// qDebug()<<"setData()";
	if(index.isValid()&&role==Qt::EditRole){
		int ir = index.row();

		beginInsertRows(index,ir,ir);
		qDebug()<<"setData:"<<value;
		endInsertRows();

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
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return true;
} 
bool PageModel::append(){
	int rc = rowCount();

	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(new TextBlock(this));
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return true;
} 
bool PageModel::append(QVariant blockType){
	int rc = rowCount();
	Block *b;
	if (blockType.toString()=="checkboxBlock"){
		b = new CheckboxBlock(this);
	}else if(blockType.toString()=="pageBlock"){
		//TODO: error segfault
		// append new page into pagelist
		int id=1;
		if(callback){
			id = callback(getPageData()->id);
			qDebug()<<"PModel::callback id:"<<id<<"Pdata->id: "<<getPageData()->id;
		}

		PageBlock *pb = new PageBlock(this);
		pb->setPageId(id);
		b = pb;
	}
	else{
		b = new TextBlock(this);
	}

	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(b);
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return true;
} 
bool PageModel::insert(QVariant blockType, const int _index){
	int rc = _index;
	Block *b;
	if (blockType.toString()=="checkboxBlock"){
		b = new CheckboxBlock(this);
	}else{
		b = new TextBlock(this);
	}

	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.insert(rc,b);
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return true;

}
void PageModel::removeRow(const int index){
	int ir = index;
	if(ir>=rowCount() ||ir<0){
		qDebug()<<"index: "<<ir<<" out of range";
		return;
	}

	this->beginRemoveRows(QModelIndex(),ir,ir);
	blockList.removeAt(ir);
	this->endRemoveRows();
}

QByteArray PageModel::listToJson(){
	QJsonArray array;
  QJsonObject obj;
	// obj.insert("PageName","PKM_03");
	// array.append(obj);

	for(auto item:blockList){
			array.append(item->blockToJson());
	}
	obj["blockList"]=array;

	QJsonDocument doc(obj);
	QByteArray res = doc.toJson(QJsonDocument::Compact);
	qDebug()<<"res: "<<res;
	return res;
}

void PageModel::parseJson(QByteArray input){
	QJsonDocument doc = QJsonDocument::fromJson(input);
	QJsonArray array;
	if(doc["blockList"].isArray())
		array= doc["blockList"].toArray();
	// qDebug()<<"array: "<<array;
	int r = rowCount()-1;

	this->beginRemoveRows(QModelIndex(),0,r);
	blockList.clear();
	this->endRemoveRows();
	emit dataChanged(index(0,0), index(r,0), {Qt::EditRole});
	// qDebug()<<"after clear: "<<rowCount();

	for(auto item: array){
		auto obj = item.toObject();
		// qDebug()<<"obj: "<< obj;
		if(!obj.contains("type"))
			continue;
		QString blockType = obj["type"].toString();
		// qDebug()<<"type: "<<blockType;

		Block *b;
		QJsonObject content = obj["content"].toObject();
		if(blockType=="textBlock"){
			TextBlock *tb = new TextBlock(this);
			tb->setText(content["text"].toString());
			b=tb;
			// qDebug()<<"text: "<<tb->text();
		}else if(blockType=="checkboxBlock"){
			CheckboxBlock *cb = new CheckboxBlock(this);
			cb->setText(content["text"].toString());
			cb->setState(content["state"].toVariant());
			// qDebug()<<"statejson:" <<content["state"].toVariant();
			b=cb;
		}else if(blockType=="pageBlock"){
			//TODO: add pageBlock parsing
			PageBlock * pb = new PageBlock(this);
			pb->setPageId(content["id"].toInt());
			b=pb;
		}
		else{
			return;
		}
		int rc = rowCount();
		this->beginInsertRows(QModelIndex(),rc,rc);
		blockList.append(b);
		// qDebug()<<"append";
		this->endInsertRows();
		emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
		// delete(b);
	}
	// qDebug()<<"parse";
}

void PageModel::parseJson(QString input){
	PageModel::parseJson(input.toUtf8());
}

QVariant PageModel::getLogic(const int _index){
	QModelIndex in = index(_index,0);
	return data(in,LogicBlockRole);
}

PageData* PageModel::getPageData(){
	// qDebug()<<"getPageData: id: "<<this->pageData->id;
	// PageData *pd = pageData;
	if(pageData){
		// qDebug()<<"PModel::getPageData pd.id: "<<pageData->id<<" pid: "<<pageData->parentId;
		return pageData;
	}else{
		PageData *pd = new PageData{-1,-1,"",""};
		return pd;
	}
}

void PageModel::setPageData(PageData* pd){
	pageData = pd;
	qDebug()<<"PModel::setPageData: id"<<pd->id <<" pid:" <<pd->parentId;
}
