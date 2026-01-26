#include <qtmetamacros.h>
#include <uiModel.h>
#include <QUrl>

	QString UiModel::readFile(const QString &oldPath){
		QUrl url(oldPath);
		const QString &path = url.toLocalFile();


		qDebug()<<"My| readfile()"<<path;
		QFile file(path);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
			QTextStream stream(&file);
			return stream.readAll();
		}
		return "Error";

	}
	void UiModel::writeFile(const QString &path,const QString input){
		QFile file(path);
		if(file.open(QFile::WriteOnly | QFile::Truncate)){
			QTextStream out(&file);
			out << input;
		}
	}
