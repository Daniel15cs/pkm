#include "mylistmodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <uiModel.h>
#include <mylistmodel.h>
QString readFile(const QString &oldPath);

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	UiModel model;
	MyListModel myListModel(nullptr);
	myListModel.append();

	qRegisterMetaType<Block*>("Block*");
	qmlRegisterUncreatableType<Block>("my",1,0,"Block","");    

	engine.rootContext()->setContextProperty("uiModel",&model);
	engine.rootContext()->setContextProperty("myListModel",&myListModel);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");
	return app.exec();
}
