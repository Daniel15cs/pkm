#include "BlockSystem/toggleBlock.h"
#include "BlockSystem/block.h"
#include <qitemselectionmodel.h>
#include <qjsonobject.h>

ToggleBlock::ToggleBlock() :Block(this)//, m_model(new PageModel(this))
{ }
QPointer<PageModel> ToggleBlock::model()const{
	return m_model;
}
QVariant ToggleBlock::getData() const {
	return QVariant();
}
QString ToggleBlock::typeName() const{
	return "toggleBlock";
}

QJsonObject ToggleBlock::blockToJson()const {
	return QJsonObject();
}
