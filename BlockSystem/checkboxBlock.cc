#include "checkboxBlock.h"
// #include "mylistmodel.h"
#include <qjsonobject.h>
#include <qvariant.h>
QString CheckboxBlock::text()const{
	return m_text;
}
int CheckboxBlock::checkState()const{
	return m_checkState;
}
void CheckboxBlock::textChanged(const QString &newText){
	m_text = newText;
}
void CheckboxBlock::stateChanged(const QVariant state){
	int st = state.toInt();
	if(st==1)
			m_checkState = 1;
	else if(st==2)
			m_checkState = 2;
	else
			m_checkState = 0;
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
		{"type",typeName()},{"state",m_checkState}, {"text",text()}});
}
