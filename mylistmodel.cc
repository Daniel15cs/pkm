#include <filesystem>
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
		case ListDataRole:{
			return blockList.at(index.row())->num;
		}
		case Qt::DisplayRole|| Qt::EditRole:{
			return blockList.at(index.row())->getData().toString();
		}
		case TextBlockRole :{
			// qDebug()<<"textBlockrole";
			return blockList.at(index.row())->getData().toString();
		}
		case LogicBlockRole:{
			QObject* obj = blockList.at(index.row());
			return QVariant::fromValue(obj);
		}
		case TypeNameRole:{
			return QVariant::fromValue(blockList.at(index.row())->typeName());
		}
		default:
		return QVariant();
	}
}
// QVariant headerData()
QVariant MyListModel::displayData(const QModelIndex &index){
	return data(index,TextBlockRole);
}

bool MyListModel::setData(const QModelIndex &index, const QVariant &value, const int role) {
	qDebug()<<"setData()";
	if(index.isValid()&&role==Qt::EditRole){
		int ir = index.row();
		beginInsertRows(index,ir,ir);
		// stringList.replace(ir, value.toString());
		qDebug()<<"setData:"<<value;
		endInsertRows();

		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

bool MyListModel::append(Block *b){
	int rc = rowCount();

	this->beginInsertRows(QModelIndex(), rc,rc);
	// qDebug()<<b->getData();
	blockList.append(b);
	this->endInsertRows();
	emit dataChanged(index(rc,0),index(rc,0),{Qt::EditRole});

	// if(stringList.at(rowCount()-1)==string){
	// 	return true;
	// 	QModelIndex ind = index(rowCount()-1);
	// }
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
		
QHash<int,QByteArray> MyListModel::roleNames() const {
	QHash<int,QByteArray> roles;
	roles[ListDataRole] = "num";
	roles[TextBlockRole] = "text";
	roles[LogicBlockRole] = "logicobject";
	roles[TypeNameRole] = "typename";
	return roles;
}
Qt::ItemFlags MyListModel::flags(const QModelIndex &index)const{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}
void MyListModel::removeRow(const int index){
	int ir = index;
	this->beginRemoveRows(QModelIndex(),ir,ir);
	blockList.removeAt(ir);
	this->endRemoveRows();
}
// void MyListModel::setModelData(const QStringList& data)
// {
//     beginResetModel();
//      = data;
//     endResetModel();
// }
