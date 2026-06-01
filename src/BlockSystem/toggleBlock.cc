#include "BlockSystem/toggleBlock.h"
#include "BlockSystem/block.h"
#include <qjsonobject.h>

	QVariant ToggleBlock::getData() const {
		return QVariant();
	}
	QString ToggleBlock::typeName() const{
		return "toggleBlock";
	}
	// void ToggleBlock::setText(const QString &newText){
	//
	// }
	QJsonObject ToggleBlock::blockToJson()const {
		return QJsonObject();
	}
