#ifndef UIMODEL_H
#define UIMODEL_H

#include <QtQml>
#include <QObject>
#include <filemodel.h>

class UiModel : public QObject{
	Q_OBJECT
	QML_ELEMENT

public:
	explicit UiModel (QObject* parent =0 ): QObject(parent){}
};

#endif
