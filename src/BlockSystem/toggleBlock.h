#include <QString>
#include <qtmetamacros.h>
#include "block.h"
#pragma once
class ToggleBlock : public Block{
	Q_OBJECT
	// Q_PROPERTY (QString text_p READ text CONSTANT);
public: 
	using Block::Block;
	// Q_INVOKABLE QString text()const;
	QVariant getData() const override;
	QString typeName()const override;
	// Q_INVOKABLE void setText(const QString &newText);
	QJsonObject blockToJson()const override;
private:
	// QString m_text;
};
