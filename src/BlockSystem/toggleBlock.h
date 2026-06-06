#include <QString>
#include <qtmetamacros.h>
#include "block.h"
#pragma once
class ToggleBlock : public Block{
	Q_OBJECT
public: 
	Q_PROPERTY(bool p_expanded READ expanded WRITE setExpanded NOTIFY expandedChanged)
	Q_PROPERTY(QString p_text READ text WRITE setText NOTIFY textChanged)
	// Q_PROPERTY(PageData* p_pageData READ getPageData CONSTANT)
	ToggleBlock();
	using Block::Block;
	QVariant getData() const override;
	QString typeName()const override;
	Q_INVOKABLE void setText(const QString &newText);
	Q_INVOKABLE QString text();
	QJsonObject blockToJson()const override;

	bool expanded()const;
	void setExpanded(bool);
signals:
	void expandedChanged();
	void textChanged();
	

private:
	QString m_text;
	bool m_expanded=true;


};
