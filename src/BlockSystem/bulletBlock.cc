#include "bulletBlock.h"
#include "BlockSystem/block.h"
#include <qjsonobject.h>
#include <qvariant.h>
QString BulletBlock::text()const{
	return m_text;
}
void BulletBlock::setText(const QString &newText){
	m_text = newText;
}
QVariant BulletBlock::getData()const{
	QVariant res = text();
	return res;
}
QString BulletBlock::typeName()const{
	return "bulletBlock";
}
QJsonObject BulletBlock::blockToJson()const{
	QJsonObject content;
	content["text"]=text();
	return Block::prepToJson(content);
}
