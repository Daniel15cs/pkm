#include<QString>
#include <QVariant>
#include <QObject>
#include <qtmetamacros.h>
#include <QJsonObject>
#pragma once
	
class Block: public QObject{
	Q_OBJECT
public:
	explicit Block(QObject *parent):QObject(parent){}
	int num;
	virtual QVariant getData()const=0;
	Q_INVOKABLE virtual QString typeName()const=0;
	virtual QJsonObject blockToJson()const=0;
};
