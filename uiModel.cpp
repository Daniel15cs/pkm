#include <qtmetamacros.h>
#include <uiModel.h>
#include <QUrl>

QString urlToString(const QString &_qurl);

QString UiModel::readFile(const QString &oldPath){
	QString path = urlToString(oldPath);
	qDebug()<<"My| readfile()"<<path;
	QFile file(path);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream stream(&file);
		return stream.readAll();
	}
	return "Error";

}
void UiModel::writeFile(const QString &url,const QString input){
	QString path = urlToString(url);
	QFile file(path);
	if(file.open(QFile::WriteOnly | QFile::Truncate)){
		QTextStream out(&file);
		out << input;
	}
}
QString urlToString(const QString &_qurl){
	QUrl url(_qurl);
	const QString &path = url.toLocalFile();
	return path;
}
