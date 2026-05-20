// #include <filesystem>
#include "BlockSystem/checkboxBlock.h"
#include "BlockSystem/pageBlock.h"
#include <mylistmodel.h>

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
int MyListModel::rowCount(const QModelIndex &parent) const{
	return blockList.count();
}
QVariant MyListModel::data(const QModelIndex &index, int role) const{
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

QHash<int,QByteArray> MyListModel::roleNames() const {
	QHash<int,QByteArray> roles;
	roles[LogicBlockRole] = "logicobject";
	return roles;
}
bool MyListModel::setData(const QModelIndex &index, const QVariant &value, const int role) {
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

Qt::ItemFlags MyListModel::flags(const QModelIndex &index)const{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
bool MyListModel::append(Block *b){
	int rc = rowCount();

	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(b);
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return false;
} 
bool MyListModel::append(){
	int rc = rowCount();

	this->beginInsertRows(QModelIndex(), rc,rc);
	blockList.append(new TextBlock(this));
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return true;
} 
bool MyListModel::append(QVariant blockType){
	int rc = rowCount();
	Block *b;
	if (blockType.toString()=="checkboxBlock"){
		b = new CheckboxBlock(this);
	}else if(blockType.toString()=="pageBlock"){
		b = new PageBlock(this);
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
bool MyListModel::insert(QVariant blockType, const int _index){
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
void MyListModel::removeRow(const int index){
	int ir = index;
	if(ir>=rowCount() ||ir<0){
		qDebug()<<"index: "<<ir<<" out of range";
		return;
	}

	this->beginRemoveRows(QModelIndex(),ir,ir);
	blockList.removeAt(ir);
	this->endRemoveRows();
}

QByteArray MyListModel::listToJson(){
	QJsonArray array;
  QJsonObject obj;

	obj.insert("PageName","PKM_03");
	array.append(obj);

	for(auto item:blockList){
			array.append(item->blockToJson());
	}
	QJsonDocument doc(array);
	return doc.toJson(QJsonDocument::Indented);
}

void MyListModel::parseJson(QByteArray input){
	QJsonArray array = QJsonDocument::fromJson(input).array();
	qDebug()<<"array: "<<array;
	int r = rowCount();

	this->beginRemoveRows(QModelIndex(),0,r);
	blockList.clear();
	this->endRemoveRows();
	emit dataChanged(index(0,0), index(r,0), {Qt::EditRole});

	qDebug()<<"after clear: "<<rowCount();

	for(auto item: array){
		auto obj = item.toObject();
		QString blockType;
		qDebug()<<"obj: "<< obj;

		if(!obj.contains("type"))
			continue;

		blockType = obj.value("type").toString();
		qDebug()<<"type: "<<blockType;

		Block *b;
		if(blockType=="textBlock"){
			TextBlock *tb = new TextBlock(this);
			tb->textChanged(obj.value("text").toString());
			b=tb;
			qDebug()<<"text: "<<tb->text();

		}else if(blockType=="checkboxBlock"){
			CheckboxBlock *cb = new CheckboxBlock(this);
			cb->textChanged(obj.value("text").toString());
			cb->stateChanged(obj.value("state"));
			qDebug()<<"statejson:" <<obj.value("state");
			b=cb;
		}else{
			return;
		}
		
		int rc = rowCount();
		this->beginInsertRows(QModelIndex(),rc,rc);
		blockList.append(b);
		qDebug()<<"append";
		this->endInsertRows();
		emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
		// delete(b);
	}
	qDebug()<<"parse";

}
QVariant MyListModel::getLogic(const int _index){
	QModelIndex in = index(_index,0);
	return data(in,LogicBlockRole);
}
