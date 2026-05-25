// #define PAGEMANAGER_H
#pragma once
#include "filemodel.h"
#include "PageModel.h"
#include <qobject.h>
#include <qtmetamacros.h>
	// Q_PROPERTY (PageModel* pageModel READ model CONSTANT);
	// Q_PROPERTY (QString pageName READ getPageName WRITE setPageName NOTIFY pageNameChanged);
// class Page: public QObject{
struct Page{
	Q_GADGET
	// Q_OBJECT
	// Q_PROPERTY (PageModel* model MEMBER model);
public:
	// Q_PROPERTY (PageData p_data MEMBER data);
	// explicit Page (QObject *parent = nullptr, PageData pd=PageData(),PageModel *m =new PageModel()) : QObject(parent){}
	PageData data;
	PageModel *model;
	int id;
};
// Q_DECLARE_METATYPE(Page)

class PageManager: public QObject{
	Q_OBJECT
private:
	QVector<Page> pagesList;
	FileModel *fileModel;
	PageData currentPage;
public:
	// explicit PageManager (QObject *parent = nullptr) : QObject(parent){}
	PageManager();
	Q_INVOKABLE void setFileModel(FileModel*);
	Q_INVOKABLE void uploadList();
	void setCurrentPage(Page);
	Q_INVOKABLE	Page getCurrentPage();
	Page* getPageById(int id=0);

	
	// {
	// 	Page page();
	// 	page.data={};
	// 	page.model.parseJson(page.data.content);
	// }
};
