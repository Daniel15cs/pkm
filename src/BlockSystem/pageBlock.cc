#include "pageBlock.h"
#include "PageModel.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
QVariant PageBlock::getData()const{
	return QVariant();
}
QString PageBlock::typeName()const{
	return "pageBlock";
}
QJsonObject PageBlock::blockToJson()const{
	QJsonObject obj;
	obj["id"]=pageId;
	return prepToJson(obj);
}
void PageBlock::setPageId(int id){
	if(pageId == id) return;
	pageId = id;
	emit pageIdChanged();
}
int PageBlock::getPageId()const{
	return pageId;
}
