#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <uiModel.h>
// #include <qfiledevice.h>
// #include <qdir.h>
QString readFile(const QString &oldPath);

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;


	UiModel model;
	qmlRegisterType<UiModel>("my",1,0,"UiModel");

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");

	qDebug()<<"My| start in main";
	QString res = "file://home/daniel/Desktop/supertext.txt";
	qDebug()<<"My| file: "<<model.readFile(res);

	return app.exec();
}
