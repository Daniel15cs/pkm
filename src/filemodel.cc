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
//TODO: crud for sqlite


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
	QString s, query="select id,content from Notes where id=?;";
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
bool FileModel::idCheckInDb(int _id){
	QSqlQuery q;
	q.prepare("select id from Notes where id=:id limit 1;");
	q.bindValue(":id",_id);
	if(!q.exec()) qDebug()<<"idCheckInDb: " <<q.lastError().text();
	if(q.next()){
		return true;
	}
	return false;
}
bool FileModel::updateListToDb(QVector<PageData> list){
		// TODO: update Notes set blabla=blalba where id=blabla 
	for(auto item:list){
		QSqlQuery q; 
		QString query;
		if(idCheckInDb(item.id)){
			query="update Notes set parentId=:parentId, type=:type, content=:content where id=:id";
			q.prepare(query);
			q.bindValue(":id",item.id);
			qDebug()<<"checkid true";
		}else{
			query="insert into Notes(parentId,type,content) values(:parentId,:type,:content)";
			q.prepare(query);
			qDebug()<<"checkid false";
		}
		int parentId= item.parentId;
		QString type= item.type=="" ? "note" : item.type;
		QString content= item.content;

		q.bindValue(":parentId",parentId);
		q.bindValue(":type",type);
		//TODO: content check for injections
		q.bindValue(":content",content);

		if(!q.exec()){
			qDebug()<<"saveListTodb error: "<<q.lastError().text();
			return false;
		}

	}
	return true;
}
