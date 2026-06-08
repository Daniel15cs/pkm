#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "filemodel.h"
#include "PageManager.h"

class FileModel;

struct PropertySchema {
    int id;
    QString name;
    QString type;
    QStringList values;
};

class DatabaseModel : public QAbstractTableModel {
    Q_OBJECT
    Q_PROPERTY(QVariantList schema READ schema NOTIFY schemaChanged)
    Q_PROPERTY(PageManager* manager READ manager WRITE setManager NOTIFY managerChanged)

public:
    enum DatabaseRoles {
        PageIdRole = Qt::UserRole + 1,
        PageDataRole,
        PropertyRole
    };

    explicit DatabaseModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void load(int parentId, const QByteArray &dbContent);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void addNote();
    Q_INVOKABLE QVariant getProperty(int row, int propertyId) const;
    Q_INVOKABLE void setProperty(int row, int propertyId, const QVariant &value);
    Q_INVOKABLE QVariant getPropertyByPageId(int pageId, int propertyId) const;
    Q_INVOKABLE void setPropertyByPageId(int pageId, int propertyId, const QVariant &value);
    Q_INVOKABLE int getPropertyIdByName(const QString &name) const;

    QVariantList schema() const;
    PageManager* manager() const { return m_PageManager; }
    void setManager(PageManager*);

signals:
    void schemaChanged();
    void managerChanged();

private:
    void parseSchema(const QByteArray &dbContent);
    QVariant getPropertyValue(const PageData &page, int propertyId) const;

    int m_parentId;
    QVector<PageData> m_childPages;
    QVector<PropertySchema> m_schema;
    QByteArray m_lastContent;
		PageManager * m_PageManager;
};

#endif // DATABASEMODEL_H
