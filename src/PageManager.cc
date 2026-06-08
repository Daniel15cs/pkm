#include "PageManager.h"
#include "filemodel.h"
#include <filesystem>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qlogging.h>

#include "DatabaseModel.h"
#include <QDateTime>

void PageManager::setFileModel(FileModel *fm){
	this->fileModel = fm;
}

int PageManager::appendPageToList(int parentId, QString type){
	int maxId = 0;
	for(const auto& p : pagesList) {
		if(p.data.id > maxId) maxId = p.data.id;
	}
	qDebug()<<"max id: "<<maxId;
	
	PageData *pd = new PageData();
	pd->id = maxId + 1;
	qDebug()<<"pdId: "<<pd->id;
	pd->parentId= parentId;
	pd->type = type;
	pd->createdAt = QDateTime::currentDateTime().toString("dd.MM.yyyy");

	PageModel  *pm =new PageModel(this);
	pm->setPageData(pd);
	pm->callback = [&](int parentId, QString type)->int{
		qDebug()<<"PManager::callback parentId: "<<parentId;
		return this->appendPageToList(parentId, type);
	};

	DatabaseModel *dbm = new DatabaseModel(this);
	dbm->setManager(this);
	dbm->load(pd->id, QByteArray());

	Page p = {*pd, pm, dbm};
	this->pagesList.append(p);
	emit pagesChanged();
	return pd->id;
}
void PageManager::uploadList(){ // parse from raw sqlite data to model
	qDebug()<<"uploadList";
	this->pagesList.clear();
	for(PageData item : fileModel->getPagesListFromSql()){
		PageData *pd = new PageData(item);

		PageModel *pm = new PageModel(this);
		pm->setPageData(pd);
		pm->callback = [&](int parentId, QString type)->int{
			qDebug()<<"callback";
			return this->appendPageToList(parentId, type);
		};

		DatabaseModel *dbm = new DatabaseModel(this);
		dbm->setManager(this);

		QString contentStr = fileModel->getPageContentFromSql(item.id);
		pm->parseJson(contentStr);

		if(item.type == "DataBase") {
			dbm->load(item.id, contentStr.toUtf8());
		}

		Page p = {item, pm, dbm};
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
	else{
		qDebug()<<"pageManager constr: pagesList is empty";
		appendPageToList();
		currentPage = pagesList[0];
	}
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

	qDebug()<<"saveList: " << fileModel->updateListToDb(pdList);
}

Page PageManager::getPageById(int id){
	for(Page item:pagesList){
		if(item.data.id==id){
			return item;
		}
	}
	return Page();
}

QVector<PageData> PageManager::getPagesByParent(int parentId) {
    QVector<PageData> result;
    for (const auto &p : pagesList) {
        if (p.data.parentId == parentId) {
            result.append(p.data);
        }
    }
    return result;
}

void PageManager::updatePageContent(int pageId, const QByteArray &content) {
    for (int i = 0; i < pagesList.size(); ++i) {
        if (pagesList[i].data.id == pageId) {
            pagesList[i].data.content = content;
            if (fileModel) {
                fileModel->setDataToSql(pagesList[i].data, pageId);
            }
            break;
        }
    }
}
