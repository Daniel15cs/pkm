#include "block.h"
#include "PageModel.h"
#include <qtmetamacros.h>
#include <qvariant.h>
#pragma once
class PageBlock : public Block{
	Q_OBJECT
	Q_PROPERTY (PageModel* pageModel READ model CONSTANT);
	Q_PROPERTY (QString pageName READ getPageName WRITE setPageName NOTIFY pageNameChanged);
public: 
	// Q_INVOKABLE QString text()const;
	// Q_INVOKABLE void textChanged(const QString &newText);
	PageBlock();
	// explicit PageBlock(QObject *parent = nullptr):Block(parent){}
	// PageBlock(QPointer<PageModel>);
	using Block::Block;
	QVariant getData() const override;
	QString typeName()const override;
	QJsonObject blockToJson()const override;
	Q_INVOKABLE QPointer<PageModel> model()const;
	Q_INVOKABLE QString getPageName();
	Q_INVOKABLE void setPageName(QString);
signals:
	void pageNameChanged(QString);
private:
	const QPointer<PageModel> m_model = new PageModel(this);
	QString m_pageName = "inlinePage";
};
