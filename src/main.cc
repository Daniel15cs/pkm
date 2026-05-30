#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <PageModel.h>
#include <filemodel.h>
#include <BlockSystem/pageBlock.h>
#include "PageManager.h"

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	QDir dir;
	QString _dbPath = dir.currentPath()+"/../../pkm.db";
	FileModel *fileModel = new FileModel();
	fileModel->openDb(_dbPath);
	
	qRegisterMetaType<Block*>("Block*");
	qmlRegisterUncreatableType<Block>("my",1,0,"Block","");    

	qRegisterMetaType<Page>("page");
	qRegisterMetaType<PageData>("PageData");

	PageManager *pageManager = new PageManager(fileModel);
	// pageManager->setFileModel(fileModel);

	// qmlRegisterType<PageBlock>("myPageBlock",1,0,"PageBlock");
	qmlRegisterType<Page>("myPage",1,0,"page");
	qmlRegisterType<PageData>("myPage",1,0,"pageData");
	
	QQmlEngine::setObjectOwnership(pageManager, QQmlEngine::CppOwnership); 
	
	engine.rootContext()->setContextProperty("p_pageManager",pageManager);
	// engine.rootContext()->setContextProperty("filemodel",fileModel);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");
	return app.exec();
}
