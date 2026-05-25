#include <qlogging.h>
#include <qsqlquery.h>
#include <qtmetamacros.h>
#include "filemodel.h"

#include <QDebug>
#include <QUrl>

QString urlToString(const QString &_qurl){
	QUrl url(_qurl);
	const QString &path = url.toLocalFile();
	return path;
}

QString FileModel::readFile(const QString &oldPath){
	QString path = urlToString(oldPath);
	qDebug()<<"My| readfile()"<<path;
	QFile file(path);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream stream(&file);
		return stream.readAll();
	}
	return "Error";

}
void FileModel::writeFile(const QString &url,const QString input){
	QString path = urlToString(url);
	QFile file(path);
	if(file.open(QFile::WriteOnly | QFile::Truncate)){
		QTextStream out(&file);
		out << input;
	}
}
//new sqlite operators
	//TODO: open sqlite db and parse it to cpp model
	//TODO: crud for sqlite
	//TODO: parse cpp model to PageModel
	// ?rename PageModel to pageModel?


void FileModel::openDb(){
	QDir dir;
	QString _dbPath = dir.currentPath()+"/../../pkm.db";
	db = QSqlDatabase::addDatabase("QSQLITE");
	if (_dbPath==""){
		qDebug()<<"opendb error: dbPath is null";
		return;
	}
	db.setDatabaseName(_dbPath);
		if(!db.open()) 
			qDebug()<<"db error: "<< db.lastError().text();
}
QVector<PageData> FileModel::getPagesListFromSql(){
		QSqlQuery q;
		QVector<PageData> notesList;
		QString query ="select * from Notes;";
		if(!q.exec(query)){
			qDebug()<<"parse to list Error: "<<q.lastError().text();
			return QVector<PageData>();
		}
		while(q.next()){
			int id = q.value("id").toInt();
			int parentId = q.value("parentId").toInt();
			QString type = q.value("type").toString();

			// PageData n = PageData(this, id,parentId,type);
			PageData n = {id,parentId,type};
			notesList.append(n);
		}
	return notesList;
}
PageData FileModel::getPageDataFromSql(int id){
	return PageData();
}
QString FileModel::getPageContentFromSql(int id){
	QSqlQuery q;
	QString s, query="select id,content from Notes where id=?";
	q.prepare(query);
	q.bindValue(0,id);
	
	if(!q.exec()){
		qDebug()<<"getPageContent error"<<q.lastError().text();
		return QString();
	}else {
		q.next();
	}
	s = q.value("content").toString();
	return s;

}
