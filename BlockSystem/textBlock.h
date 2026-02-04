#include <QString>
#include <qtmetamacros.h>
#include "block.h"
#pragma once
// #include <QMetaType>
class TextBlock : public Block{
	Q_OBJECT
	Q_PROPERTY (QString text_p READ text CONSTANT);
public: 
	Q_INVOKABLE QString text()const;
	QVariant getData() const override;
	QString typeName()const override;
	QString m_text;
	Q_INVOKABLE void textChanged(const QString &newText);
// private:
};
// Q_DECLARE_METATYPE(TextBlock)
