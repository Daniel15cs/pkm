// #define PAGEMANAGER_H
#pragma once
#include "filemodel.h"
#include "PageModel.h"
#include <qobject.h>
#include <qtmetamacros.h>
struct Page{
	Q_GADGET
	Q_PROPERTY (PageData p_data MEMBER data);
	Q_PROPERTY (PageModel* model MEMBER model);
public:
	PageData data;
	PageModel *model = new PageModel;
};
Q_DECLARE_METATYPE(Page)

class PageManager: public QObject{
	Q_OBJECT
	Q_PROPERTY(Page p_currentPage READ getCurrentPage NOTIFY currPageChanged)
private:
	QVector<Page> pagesList;
	FileModel *fileModel;
	Page currentPage,lastPage;
public:
	// explicit PageManager (QObject *parent = nullptr) : QObject(parent){}
	PageManager(FileModel*);
	Q_INVOKABLE void setFileModel(FileModel*);
	Q_INVOKABLE void uploadList();
	Q_INVOKABLE void setCurrentPage(int id=0);
	Q_INVOKABLE	Page getCurrentPage();
	Page getPageById(int id=0);
	Q_INVOKABLE void savePagesList();
	Q_INVOKABLE void getToLastPage();
	Q_INVOKABLE int appendPageToList(int parentId=0);

signals:
	void currPageChanged(Page);
};
