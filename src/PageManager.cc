#include "PageManager.h"
#include "filemodel.h"
#include <filesystem>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qlogging.h>

void PageManager::setFileModel(FileModel *fm){
	this->fileModel = fm;
}

int PageManager::appendPageToList(int parentId){
	PageData *pd = new PageData();
	pd->id=pagesList.length()+1;
	pd->parentId= parentId;
	PageModel  *pm =new PageModel(this);
	// qDebug()<<"PManager::appendPageToList()PD: id"<<pd->id<<" pid: "<<pd->parentId;
	pm->setPageData(pd);
	pm->callback = [&](int parentId)->int{
		// qDebug()<<"PManager::callback parentId: "<<parentId;
		return this->appendPageToList(parentId);
	};
	// qDebug()<<"PManager::appendPageToList()PModel: id" << pm->getPageData()->id << " pid: " << pm->getPageData()->parentId;
	Page p = {*pd,pm};
	this->pagesList.append(p);
	return pd->id;
}
void PageManager::uploadList(){ // parse from raw sqlite data to model
	this->pagesList.clear();
	for(PageData item : fileModel->getPagesListFromSql()){
		PageModel *pm = new PageModel(this);
		PageData *pd = new PageData(item);

		pm->setPageData(pd);
		pm->parseJson(fileModel->getPageContentFromSql(item.id));
		pm->callback = [&](int parentId)->int{
			return this->appendPageToList(parentId);
		};

		Page p = {item,pm};
		this->pagesList.append(p);
	} 
	if(!pagesList.isEmpty()) setCurrentPage(pagesList[0].data.id);
}

PageManager::PageManager(FileModel* fm){
	// fileModel->openDb();
	fileModel =fm;
	this->uploadList();
//debug for input raw data
	// for(auto item:pagesList){
	// 	auto d = item.data;
	// 	auto m = item.model;
	// 	qDebug()<<"data: id:"<<d.id<<": pId"<<d.parentId<<": type"<<d.type;
	// 	qDebug()<<"rowCount: "<< m->rowCount();
	// 	qDebug()<<"content: "<<fileModel->getPageContentFromSql(d.id);
	// }
	if(!pagesList.isEmpty())
		currentPage = pagesList[0];
	else
		qDebug()<<"pageManager constr: pagesList is empty";
		// currentPage = Page();
		// pagesList.append(currentPage);
		// pagesList.append(Page());
		// currentPage = pagesList[0];
}
void PageManager::setCurrentPage(int id){
		lastPage = currentPage;
		for(auto item:pagesList) {
			if(item.data.id==id){
				currentPage=item;
				emit currPageChanged(currentPage);
				// qDebug()<<"setCurrentPage id: "<<id;
			}
		}
		// currentPage = pagesList.at(id);
}

Page PageManager::getCurrentPage(){
	for(auto item: pagesList){
		if(item.data.id==currentPage.data.id){
			// qDebug()<<"currpage:" <<item.data.id;
			return item;
		}
	}
	return Page();
}
void PageManager::getToLastPage(){
	// setCurrentPage(lastPage.data.id);
	setCurrentPage(currentPage.data.parentId);
}
void PageManager::savePagesList(){
	QVector<PageData> pdList;
	
	for(auto item:pagesList){
		PageData pd = item.data;
		pd.content = item.model->listToJson();
		// qDebug()<<pd.content;
		pdList.append(pd);
	}

	// qDebug()<<"saveList: " << fileModel->updateListToDb(pdList);
}

Page PageManager::getPageById(int id){
	for(Page item:pagesList){
		if(item.data.id==id){
			return item;
		}
	}
	return Page();
}
