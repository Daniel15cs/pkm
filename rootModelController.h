#include <QObject>
#include <qtmetamacros.h>
#include "BlockSystem/pageBlock.h"
#pragma once

class RootModelController : public QObject{
	Q_OBJECT
	Q_PROPERTY(PageBlock* p_rootPage READ getRootPage NOTIFY rootPageChanged)
	// Q_PROPERTY(PageBlock* p_oldRootPage READ getOldRootPage)
public:
	// explicit RootModelController(QObject* parent = nullptr);
	PageBlock* getRootPage();
	PageBlock* getOldRootPage();
	Q_INVOKABLE void setRootPage(PageBlock *value);
	Q_INVOKABLE void getToOldPage();
	QPointer<PageBlock> oldRootPage = new PageBlock(this);;
	QPointer<PageBlock> rootPage = oldRootPage;
signals:
	void rootPageChanged(PageBlock*);
}; 
