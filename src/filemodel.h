#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <qdir.h>
#include <qfilesystemmodel.h>
#include <qlogging.h>
#include <qqmlintegration.h>
#include <qstringview.h>
#include <qtmetamacros.h>
#include <QObject>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qdebug.h>
#include <QSqlError>


// #include <QtQml>

struct PageData{
Q_GADGET
QML_ELEMENT
public:
	Q_PROPERTY(int id MEMBER id)
	Q_PROPERTY(int parentId MEMBER parentId)
	Q_PROPERTY(QString type MEMBER type)
	int id=1, parentId=-1;
	QString type="note";
	QByteArray content;
	bool operator==(const PageData&)const =default;
	QML_VALUE_TYPE(pageData)
	
};
Q_DECLARE_METATYPE(PageData)
	//properties
	//childList maybe?

class FileModel : public QObject{
	Q_OBJECT
	QML_ELEMENT
public:
	explicit FileModel(QObject *parent = nullptr):QObject(parent){}
	
	// new sqlite operators
	//TODO: open sqlite db and parse it to cpp model
	//TODO: crud for sqlite
	//TODO: parse cpp model to 
	//
	// QVector<PageData> pageDataList;
	QString dbPath;
	QSqlDatabase db;
	QVector<PageData> getPagesListFromSql();

	void openDb();

	PageData getPageDataFromSql(int id);
	QString getPageContentFromSql(int id=0);

	void setDataToSql(PageData _data,int pageId);
	void setPageContentToSql(QString _newContent,int pageId);

	void addDataToSql(PageData _data);
	bool idCheckInDb(int);
	bool updateListToDb(QVector<PageData> list);
	

public slots:
	//old json operators
	QString readFile(const QString &oldPath);
	void writeFile(const QString &path, QString input);
	

};

#endif // FILEMODEL_H
