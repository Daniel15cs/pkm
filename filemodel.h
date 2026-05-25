#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <qdir.h>
#include <qfilesystemmodel.h>
#include <qlogging.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <QObject>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qdebug.h>
#include <QSqlError>


// #include <QtQml>

// class PageData: QObject{
// 	Q_OBJECT
struct PageData{
public:
	int id, parentId;
	QString type;
	// PageData (QObject *parent = nullptr, int id=0, int parentId=-1, QString type="note") : QObject(parent){}
	
	// QString content;
	//properties
	//childList maybe?
};
// Q_DECLARE_METATYPE(PageData)

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

	void updateDbFromList(QVector<PageData> list);
	

public slots:
	//old json operators
	QString readFile(const QString &oldPath);
	void writeFile(const QString &path, QString input);
	

};

#endif // FILEMODEL_H
