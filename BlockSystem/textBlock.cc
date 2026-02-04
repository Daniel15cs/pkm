#include "textBlock.h"
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
	return "TextBlock";
}

