#include "PageManager.h"
#include "filemodel.h"
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qlogging.h>

void PageManager::setFileModel(FileModel *fm){
	this->fileModel = fm;
}
void PageManager::uploadList(){
	// QVector<PageData> pagelist = 	
	bool listIsEmpty = this->pagesList.isEmpty();
	for(auto item : fileModel->getPagesListFromSql()){
		PageModel *pm = new PageModel(this);
		pm->parseJson(fileModel->getPageContentFromSql(item.id));
		Page p = {item,pm};
		if(listIsEmpty){
			this->pagesList.append(p);
		}else{
			//TODO: somehow save old data or ask user to rewrite it
			this->pagesList.clear();
			this->pagesList.append(p);
		}
	}
}
PageManager::PageManager(){
	// fileModel->openDb();
	this->uploadList();
	for(auto item:pagesList){
		auto d = item.data;
		auto m = item.model;
		qDebug()<<"data: id:"<<d.id<<": pId"<<d.parentId<<": type"<<d.type;
		qDebug()<<"rowCount: "<< m->rowCount();
		qDebug()<<"content: "<<fileModel->getPageContentFromSql(d.id);
	}
	if(!pagesList.isEmpty())
		currentPage = pagesList[0].data;
	else
		qDebug()<<"pageManager constr: pagesList is empty";
}

Page PageManager::getCurrentPage(){
	for(auto item: pagesList){
		if(item.data.id==currentPage.id){
			qDebug()<<"currpage:" <<item.data.id;
			return item;
		}
	}
	return Page();
}
