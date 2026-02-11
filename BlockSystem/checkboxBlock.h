
#include <QString>
#include <qtmetamacros.h>
#include "block.h"
#pragma once
class CheckboxBlock : public Block{
	Q_OBJECT
	Q_PROPERTY (QString text_p READ text CONSTANT);
	Q_PROPERTY (int checkState_p READ checkState CONSTANT);
public: 
	Q_INVOKABLE QString text()const;
	Q_INVOKABLE int checkState()const;
	QVariant getData() const override;
	QString typeName()const override;
	Q_INVOKABLE void textChanged(const QString &newText);
	Q_INVOKABLE void stateChanged(const QVariant state);
	QJsonObject blockToJson()const override;
private:
	QString m_text;
	int m_checkState=0;
};
