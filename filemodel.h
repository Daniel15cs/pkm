#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <qdir.h>
#include <qfilesystemmodel.h>
#include <qlogging.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <QObject>
// #include <QtQml>

class FileModel : public QObject{
    Q_OBJECT
		QML_ELEMENT
public:
    explicit FileModel(QObject *parent = nullptr):QObject(parent){}
public slots:
	 QString readFile(const QString &oldPath);
	 void writeFile(const QString &path, QString input);
};

#endif // FILEMODEL_H
