// #include <filesystem>
#include<mylistmodel.h>
#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <qpointer.h>
#include <qvariant.h>
#include <QDebug>
int MyListModel::rowCount(const QModelIndex &parent) const{
	return blockList.count();
}
QVariant MyListModel::data(const QModelIndex &index, int role) const{
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
	qDebug()<<"setData()";
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
	blockList.append(new TextBlock());
	this->endInsertRows();

	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});
	return true;
} 
void MyListModel::removeRow(const int index){
	int ir = index;

	this->beginRemoveRows(QModelIndex(),ir,ir);
	blockList.removeAt(ir);
	this->endRemoveRows();
}
