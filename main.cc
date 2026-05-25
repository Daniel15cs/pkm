#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <PageModel.h>
#include <filemodel.h>
#include <BlockSystem/pageBlock.h>
#include "PageManager.h"
#include "rootModelController.h"

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	FileModel *fileModel = new FileModel();
	// fileModel->dbPath = dbPath;
	fileModel->openDb();
	
	// PageModel pageModel(nullptr);
	// pageModel.append("checkboxBlock");

	qRegisterMetaType<Block*>("Block*");
	qmlRegisterUncreatableType<Block>("my",1,0,"Block","");    

	qRegisterMetaType<Page>("page");
	qRegisterMetaType<PageData>("PageData");
	// qmlRegisterUncreatableType<Page>("my",1,0,"Page","");    
	// qmlRegisterUncreatableType<PageData>("my",1,0,"PageData","");    

	//root page 
	//TODO: change this to generalPageController
	// RootModelController *controller = new RootModelController;
	// controller->oldRootPage->setPageName("root");
	
	PageManager *pageManager = new PageManager;
	pageManager->setFileModel(fileModel);

	// QPointer<PageBlock> rootmodel = new PageBlock(controller);
	// controller->oldRootPage = rootmodel;
	// controller->oldRootPage->setPageName("rootPage");
	// controller->rootPage = controller->oldRootPage;
	// controller->setRootPage(rootPage);

	// qmlRegisterType<PageBlock>("myPageBlock",1,0,"PageBlock");
	qmlRegisterType<Page>("myPage",1,0,"Page");
	
	QQmlEngine::setObjectOwnership(pageManager, QQmlEngine::CppOwnership); 
	// QQmlEngine::setObjectOwnership(rootmodel, QQmlEngine::CppOwnership); 
	
	engine.rootContext()->setContextProperty("p_pageManager",pageManager);
	// engine.rootContext()->setContextProperty("filemodel",fileModel);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");
	return app.exec();
}
