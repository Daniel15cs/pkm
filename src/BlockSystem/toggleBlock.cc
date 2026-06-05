#include "BlockSystem/toggleBlock.h"
#include "BlockSystem/block.h"
#include <qitemselectionmodel.h>
#include <qjsonobject.h>

ToggleBlock::ToggleBlock() :Block(nullptr) { }

QVariant ToggleBlock::getData() const {
	return QVariant(m_text);
}
QString ToggleBlock::text(){
	return m_text;
}
void ToggleBlock::setText(const QString &newText){
	m_text = newText;
	emit textChanged();
}
QString ToggleBlock::typeName() const{
	return "toggleBlock";
}

QJsonObject ToggleBlock::blockToJson()const {
	 QJsonObject content;
	 content["text"] = m_text;
	 content["expanded"] = m_expanded;
	 return Block::prepToJson(content);
}

bool ToggleBlock::expanded()const{
	return m_expanded;
}
void ToggleBlock::setExpanded(bool e){
	if(m_expanded==e) return;
	m_expanded=e;
	emit expandedChanged();
}
