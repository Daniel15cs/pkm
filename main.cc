#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <mylistmodel.h>
#include <filemodel.h>
#include <BlockSystem/pageBlock.h>
#include "rootModelController.h"

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	FileModel *fileModel = new FileModel(nullptr);
	// MyListModel myListModel(nullptr);
	// myListModel.append("checkboxBlock");

	qRegisterMetaType<Block*>("Block*");
	qmlRegisterUncreatableType<Block>("my",1,0,"Block","");    

	//root page 
	RootModelController *controller = new RootModelController;
	controller->oldRootPage->setPageName("root");

	// QPointer<PageBlock> rootmodel = new PageBlock(controller);
	// controller->oldRootPage = rootmodel;
	// controller->oldRootPage->setPageName("rootPage");
	// controller->rootPage = controller->oldRootPage;
	// controller->setRootPage(rootPage);

	// qmlRegisterType<PageBlock>("myPageBlock",1,0,"PageBlock");
	
	QQmlEngine::setObjectOwnership(controller, QQmlEngine::CppOwnership); 
	// QQmlEngine::setObjectOwnership(rootmodel, QQmlEngine::CppOwnership); 
	engine.rootContext()->setContextProperty("controller",controller);
	engine.rootContext()->setContextProperty("filemodel",fileModel);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
			[]() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("PKM_03_qml", "Main");
	return app.exec();
}
