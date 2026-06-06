#include<QString>
#include <QVariant>
#include <QObject>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtmetamacros.h>
#include <QJsonObject>
#pragma once
	
class Block: public QObject{
	Q_OBJECT
	Q_PROPERTY(int level READ level NOTIFY levelChanged)
public:
	explicit Block(QObject *parent):QObject(parent), m_parentBlock(nullptr){}
	QVector<Block*> children;
	virtual QVariant getData()const=0;
	Q_INVOKABLE virtual QString typeName()const=0;
	virtual QJsonObject blockToJson()const=0;

	Block* parentBlock()const{
		return m_parentBlock;
	}
	void setParentBlock(Block* p){
		m_parentBlock=p;
	}
	int level() const{
		int l= 0;
		auto p = m_parentBlock;
		while(p){
			++l;
			p= p->parentBlock();
		}
		return l;
	}
	// void setLevel(int l ){
	// 	if(m_level == l) return;
	// 	m_level = l;
	// 	emit levelChanged();
	// }
signals:
	void levelChanged();
protected:
	Block* m_parentBlock;
	virtual QJsonObject prepToJson(QJsonObject content)const{
		QJsonObject resObj;
		resObj["type"] = typeName();
		resObj["content"]=content;
		if(!children.isEmpty()){
			QJsonArray childArray;
			for(auto child : children){
				childArray.append(child->blockToJson());
			}
			resObj["children"] = childArray;
		}
		return resObj;
	}
};

