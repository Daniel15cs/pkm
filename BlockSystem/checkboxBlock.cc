#include "checkboxBlock.h"
// #include "mylistmodel.h"
#include <qjsonobject.h>
#include <qvariant.h>
QString CheckboxBlock::text()const{
	return m_text;
}
void CheckboxBlock::textChanged(const QString &newText){
	m_text = newText;
}
QVariant CheckboxBlock::getData()const{
	QVariant res = text();
	return res;
}
QString CheckboxBlock::typeName()const{
	return "checkboxBlock";
}
QJsonObject CheckboxBlock::blockToJson()const{
	return QJsonObject({
		{"type",typeName()}, {"text",text()}});
}
