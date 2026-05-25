#include "pageBlock.h"
#include "PageModel.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
PageBlock::PageBlock() :Block(this), m_model(new PageModel(this)){

	m_pageName = "inline page";
}
QString PageBlock::getPageName(){
	return m_pageName;
}
void PageBlock::setPageName(QString _value){
	m_pageName = _value;
	emit pageNameChanged(m_pageName);
}
QPointer<PageModel> PageBlock::model()const{
	return m_model;
}
QVariant PageBlock::getData()const{
	return QVariant();
}
QString PageBlock::typeName()const{
	return "pageBlock";
}
QJsonObject PageBlock::blockToJson()const{
	QJsonDocument model = QJsonDocument::fromJson( m_model->listToJson());
	QJsonArray array;
		QJsonObject modelObj;
			modelObj.insert("page",model.array());
			array.append(modelObj);
		QJsonObject nameObj;
			nameObj.insert("PageName",m_pageName);
			array.append(nameObj);

	QJsonObject res;
	res.insert("page",array);


	qDebug()<<"res:"<<res;
	// qDebug()<<"res:"<<res.array();
	// qDebug()<<"doc:"<<doc;
	// qDebug()<<"doc:"<<doc.array();
	return res;
}
