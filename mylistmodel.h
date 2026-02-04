#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H

#include <QString>
#include <QtQml>
#include <QObject>
#include <QAbstractTableModel>
#include <qabstractitemmodel.h>
#include <qcontainerfwd.h>
#include <qjsondocument.h>
#include <qnamespace.h>
#include <qtmetamacros.h>
#include "BlockSystem/textBlock.h"
#pragma once

class MyListModel : public QAbstractListModel{
	Q_OBJECT
	QML_ELEMENT
	enum ListRoles{
		LogicBlockRole = Qt::UserRole+1,
	};
public:
	explicit MyListModel (QObject *parent = nullptr): QAbstractListModel(parent){}

	int rowCount(const QModelIndex &parent = QModelIndex())const override;
	QVariant data(const QModelIndex &index, int role)const override;
	QHash<int,QByteArray> roleNames() const override;
	bool setData(const QModelIndex &index, const QVariant &value, const int role) override;
	Qt::ItemFlags flags(const QModelIndex &index)const override;

	Q_INVOKABLE bool append(Block *b);
	Q_INVOKABLE bool append();
	Q_INVOKABLE void removeRow(const int index);
	Q_INVOKABLE QByteArray listToJson();
	Q_INVOKABLE void parseJson(QByteArray input);
	//
	// Q_INVOKABLE QVariant displayData(const QModelIndex &index);
	// void setModelData(const QStringList& data);
private:
	QVector<Block*> blockList;
};

#endif
