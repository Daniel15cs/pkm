#include "block.h"
// #include "PageModel.h"
#include <qtmetamacros.h>
#include <qvariant.h>
#pragma once
class PageBlock : public Block{
	Q_OBJECT
	Q_PROPERTY (int p_pageId READ getPageId WRITE setPageId NOTIFY pageIdChanged);
public: 
	// PageBlock();
	using Block::Block;
	// PageBlock(QObject *parent=nullptr);
	QVariant getData() const override;
	QString typeName()const override;
	QJsonObject blockToJson()const override;
	void setPageId(int);
	int getPageId()const;
signals:
	void pageNameChanged(QString);
	void pageIdChanged();
private:
	int pageId=-1;
};
