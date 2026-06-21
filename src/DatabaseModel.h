#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "PageManager.h"

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

    // Schema Management
    Q_INVOKABLE void addProperty(const QString &name, const QString &type);
    Q_INVOKABLE void removeProperty(int propertyId);
    Q_INVOKABLE void renameProperty(int propertyId, const QString &newName);
    Q_INVOKABLE void addPropertyValue(int propertyId, const QString &value);
    
    // Visibility Management
    Q_INVOKABLE bool isPropertyVisible(int propertyId, const QString &viewType) const;
    Q_INVOKABLE void setPropertyVisible(int propertyId, const QString &viewType, bool visible);

    QVariantList schema() const;
    PageManager* manager() const { return m_PageManager; }
    void setManager(PageManager*);

signals:
    void schemaChanged();
    void managerChanged();
    void propertyChanged(int pageId, int propertyId, const QVariant &value);

private:
    void parseSchema(const QByteArray &dbContent);
    void saveSchema();
    void loadVisibility();
    QVariant getPropertyValue(const PageData &page, int propertyId) const;

    int m_parentId;
    QVector<PageData> m_childPages;
    QVector<PropertySchema> m_schema;
    PageManager * m_PageManager;
    QMap<QString, QSet<int>> m_visiblePropertiesCache;
};

#endif // DATABASEMODEL_H
