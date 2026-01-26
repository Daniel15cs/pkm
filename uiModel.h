#ifndef UIMODEL_H
#define UIMODEL_H

#include <qdir.h>
#include <qfilesystemmodel.h>
#include <qlogging.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <QtQml>
#include <QObject>

class UiModel : public QObject{
	Q_OBJECT
	QML_ELEMENT

public:
	explicit UiModel (QObject* parent =0 ): QObject(parent){}
public slots:
	 QString readFile(const QString &oldPath);
	 void writeFile(const QString &path, QString input);
};

#endif
