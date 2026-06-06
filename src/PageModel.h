#ifndef PAGEMODEL_H
#define PAGEMODEL_H
#pragma once

#include <QString>
#include <QtQml>
#include <QObject>
#include <QAbstractTableModel>
#include <qabstractitemmodel.h>
#include <qcontainerfwd.h>
#include <qjsondocument.h>
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include "BlockSystem/block.h"
#include "BlockSystem/textBlock.h"
#include "BlockSystem/checkboxBlock.h"
#include "BlockSystem/toggleBlock.h"
#include "BlockSystem/bulletBlock.h"
#include "filemodel.h"


class PageModel : public QAbstractListModel{
	Q_OBJECT
	QML_ELEMENT
	Q_PROPERTY(PageData* p_pageData READ getPageData CONSTANT)
	enum ListRoles{
		LogicBlockRole = Qt::UserRole+1,
	};
public:
	explicit PageModel (QObject *parent = nullptr): QAbstractListModel(parent), pageData(nullptr){}

	int rowCount(const QModelIndex &parent = QModelIndex())const override;
	QVariant data(const QModelIndex &index, int role)const override;
	QHash<int,QByteArray> roleNames() const override;
	bool setData(const QModelIndex &index, const QVariant &value, const int role) override;
	Qt::ItemFlags flags(const QModelIndex &index)const override;

	Q_INVOKABLE bool append(Block *b);
	Q_INVOKABLE bool append();
	Q_INVOKABLE bool append(QVariant blockType);

	Q_INVOKABLE bool insert(QVariant blockType, const int index);
	Q_INVOKABLE void removeRow(const int index);

	Q_INVOKABLE QByteArray listToJson();
	Q_INVOKABLE void parseJson(QByteArray);
	Q_INVOKABLE void parseJson(QString);

	Q_INVOKABLE QVariant getLogic(const int);

	Q_INVOKABLE PageData* getPageData();
	void setPageData(PageData*);
	std::function<int(int)> callback;

	// void buildVisibleList();
	void insertChildList(ToggleBlock*);
	void rmChildList(Block*);
	Q_INVOKABLE void toggle(ToggleBlock*);
	Q_INVOKABLE void addIndentBlock(Block*);
	Q_INVOKABLE void rmIndentBlock(Block*);
	void updateChildIndent(Block*);
	int countChilds(Block* block);

	// Q_INVOKABLE QVariant displayData(const QModelIndex &index);
	// void setModelData(const QStringList& data);
private:
	QVector<Block*> blockList;
	QVector<Block*> realList;
	PageData *pageData;
};

#endif
