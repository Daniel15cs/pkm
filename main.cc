#include "mylistmodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <uiModel.h>
#include <mylistmodel.h>
// #include <qfiledevice.h>
// #include <qdir.h>
QString readFile(const QString &oldPath);

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;


	UiModel model;
	MyListModel myListModel(nullptr);
	TextBlock *b = new TextBlock();
	b->textChanged("blcoktest");
	myListModel.append(b);
	myListModel.append();
	// myListModel.append("second");
	// myListModel.append("third");
	// qmlRegisterType<UiModel>("my",1,0,"UiModel");
	qRegisterMetaType<Block*>("Block*");
	qmlRegisterUncreatableType<Block>("my",1,0,"Block","");    
	engine.rootContext()->setContextProperty("uiModel",&model);
	engine.rootContext()->setContextProperty("myListModel",&myListModel);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");

	qDebug()<<"My| start in main";
	// QString res = "file://home/daniel/Desktop/supertext.txt";
	// qDebug()<<"My| file: "<<model.readFile(res);

	return app.exec();
}
