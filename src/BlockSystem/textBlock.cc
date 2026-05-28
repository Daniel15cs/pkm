#include "textBlock.h"
#include "BlockSystem/block.h"
// #include "PageModel.h"
#include <qjsonobject.h>
#include <qvariant.h>
QString TextBlock::text()const{
	return m_text;
}
void TextBlock::textChanged(const QString &newText){
	m_text = newText;
}
QVariant TextBlock::getData()const{
	QVariant res = text();
	return res;
}
QString TextBlock::typeName()const{
	return "textBlock";
}
QJsonObject TextBlock::blockToJson()const{
	QJsonObject content;
	content["text"]=text();
	return Block::prepToJson(content);
}
