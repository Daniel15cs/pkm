#include "rootModelController.h"
PageBlock* RootModelController::getRootPage() {
	return rootPage;
}
PageBlock* RootModelController::getOldRootPage(){
	return oldRootPage;
}
void RootModelController::setRootPage(PageBlock *value) {
		qDebug()<<"set: root:"<<rootPage<<" old:" <<oldRootPage;
	if(rootPage==value) return;
	// if(value){
	// 	rootPage->deleteLater();
	// 	qDebug()<<"setroot";
	// }
	
	rootPage=value;
	if(value){
		value->setParent(this);
	}
	emit rootPageChanged(value);
}
void RootModelController::getToOldPage(){
	// if(oldRootPage){
		qDebug()<<"old:  root:"<<rootPage<<" old:" <<oldRootPage;
		rootPage = oldRootPage;

		qDebug()<<"gettooldpage";
		emit rootPageChanged(oldRootPage);
	// }
}
