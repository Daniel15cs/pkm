#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <mylistmodel.h>
#include <filemodel.h>
// QString readFile(const QString &oldPath);

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

    FileModel *fileModel = new FileModel(nullptr);
	MyListModel myListModel(nullptr);
	// myListModel.append();

	qRegisterMetaType<Block*>("Block*");
	qmlRegisterUncreatableType<Block>("my",1,0,"Block","");    

	engine.rootContext()->setContextProperty("myListModel",&myListModel);
    engine.rootContext()->setContextProperty("filemodel",fileModel);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");
	return app.exec();
}
