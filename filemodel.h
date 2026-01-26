#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QObject>
#include <QtQml>

class FileModel : public QObject{
    Q_OBJECT
		QML_ELEMENT
public:
    explicit FileModel(QObject *parent = nullptr);
    Q_INVOKABLE int test();
signals:
};

#endif // FILEMODEL_H
