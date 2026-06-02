#include <QString>
#include <qtmetamacros.h>
#include "block.h"
#include "../PageModel.h"
#pragma once
class ToggleBlock : public Block{
	Q_OBJECT
	Q_PROPERTY (PageModel *p_model READ model CONSTANT);
public: 
	ToggleBlock();
	using Block::Block;
	Q_INVOKABLE QPointer<PageModel> model() const;
	QVariant getData() const override;
	QString typeName()const override;
	// Q_INVOKABLE void setText(const QString &newText);
	QJsonObject blockToJson()const override;
private:
	const QPointer<PageModel> m_model = new PageModel(this);
	// PageModel *m_model ;//= new PageModel;

	// PageModel *m_model;
};
