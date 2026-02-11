#include "block.h"
#include <QVariant>
QString Block::typeName()const{
	return "block";
}
QJsonObject Block::blockToJson()const{
	return QJsonObject({{"type",typeName()}});
}
