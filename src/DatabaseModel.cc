#include "DatabaseModel.h"
#include "PageManager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

DatabaseModel::DatabaseModel(QObject *parent)
    : QAbstractTableModel(parent), m_parentId(-1), m_PageManager(nullptr) {}

int DatabaseModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_childPages.count();
}

int DatabaseModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_schema.count() + 1; // Properties + Title (ID for now)
}

QVariant DatabaseModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_childPages.count())
        return QVariant();

    const PageData &page = m_childPages.at(index.row());

    if (role == PageIdRole) return page.id;
    if (role == PageDataRole) return QVariant::fromValue(page);

    if (role == Qt::DisplayRole || role == PropertyRole) {
        if (index.column() == 0) {
            // Try to extract title from content
            QJsonDocument doc = QJsonDocument::fromJson(page.content);
            QJsonArray blocks = doc.object()["blockList"].toArray();
            for (const auto &b : blocks) {
                QJsonObject obj = b.toObject();
                if (obj["type"].toString() == "textBlock") {
                    QString text = obj["content"].toObject()["text"].toString();
                    if (!text.isEmpty()) return text;
                }
            }
            return QString("Page %1").arg(page.id); // Fallback
        }
        
        int propertyIdx = index.column() - 1;
        if (propertyIdx >= 0 && propertyIdx < m_schema.count()) {
            return getPropertyValue(page, m_schema[propertyIdx].id);
        }
    }

    return QVariant();
}

QVariant DatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) return "Title";
        int propertyIdx = section - 1;
        if (propertyIdx >= 0 && propertyIdx < m_schema.count()) {
            return m_schema[propertyIdx].name;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> DatabaseModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractTableModel::roleNames();
    roles[PageIdRole] = "pageId";
    roles[PageDataRole] = "pageData";
    roles[PropertyRole] = "propertyValue";
    return roles;
}

void DatabaseModel::load(int parentId, const QByteArray &dbContent) {
    beginResetModel();
    m_parentId = parentId;
    m_lastContent = dbContent;
    m_childPages.clear();
    m_schema.clear();

    parseSchema(dbContent);

    if (m_PageManager) {
        m_childPages = m_PageManager->getPagesByParent(m_parentId);
    }
    endResetModel();
    emit schemaChanged();
}

void DatabaseModel::refresh() {
    load(m_parentId, m_lastContent);
}

void DatabaseModel::setManager(PageManager* p ){
    if (m_PageManager != p) {
        m_PageManager = p;
        emit managerChanged();
    }
}
void DatabaseModel::addNote() {
    if (m_PageManager && m_parentId != -1) {
        m_PageManager->appendPageToList(m_parentId, "note");
        // m_PageManager->savePagesList();
        refresh();
    }
}

QVariantList DatabaseModel::schema() const {
    QVariantList list;
    for (const auto &s : m_schema) {
        QVariantMap map;
        map["id"] = s.id;
        map["name"] = s.name;
        map["type"] = s.type;
        map["values"] = s.values;
        list.append(map);
    }
    return list;
}

void DatabaseModel::parseSchema(const QByteArray &dbContent) {
    m_schema.clear();
    QJsonDocument doc = QJsonDocument::fromJson(dbContent);
    if (dbContent.isEmpty() || doc.isNull() || !doc.isObject() || !doc.object().contains("properties")) {
        // Default schema for new databases
        PropertySchema ps;
        ps.id = 1;
        ps.name = "Status";
        ps.type = "status";
        ps.values = {"todo", "in-progress", "done"};
        m_schema.append(ps);
        emit schemaChanged();
        return;
    }

    QJsonArray props = doc.object()["properties"].toArray();
    for (const auto &val : props) {
        QJsonObject obj = val.toObject();
        PropertySchema ps;
        ps.id = obj["propertyId"].toInt();
        ps.name = obj["name"].toString();
        if (ps.name.isEmpty()) ps.name = obj["type"].toString(); // Fallback
        ps.type = obj["type"].toString();
        
        QJsonArray vals = obj["values"].toArray();
        for (const auto &v : vals) ps.values.append(v.toString());
        
        m_schema.append(ps);
    }
    emit schemaChanged();
}

QVariant DatabaseModel::getPropertyValue(const PageData &page, int propertyId) const {
    QJsonDocument doc = QJsonDocument::fromJson(page.content);
    QJsonObject root = doc.object();
    QJsonArray props = root["properties"].toArray();
    
    for (const auto &val : props) {
        QJsonObject obj = val.toObject();
        if (obj["propertyId"].toInt() == propertyId) {
            return obj["value"].toVariant();
        }
    }

    // Default values if not found in page content
    for(const auto& s : m_schema) {
        if(s.id == propertyId) {
            if (s.type == "creation_date") return page.createdAt;
            if (s.type == "status" && !s.values.isEmpty()) return s.values.first();
        }
    }

    return QVariant();
}

void DatabaseModel::setProperty(int row, int propertyId, const QVariant &value) {
    if (row < 0 || row >= m_childPages.count()) return;

    PageData &page = m_childPages[row];
    QJsonDocument doc = QJsonDocument::fromJson(page.content);
    QJsonObject root = doc.object();
    QJsonArray props = root["properties"].toArray();
    
    bool found = false;
    for (int i = 0; i < props.size(); ++i) {
        QJsonObject obj = props[i].toObject();
        if (obj["propertyId"].toInt() == propertyId) {
            obj["value"] = QJsonValue::fromVariant(value);
            props[i] = obj;
            found = true;
            break;
        }
    }

    if (!found) {
        QJsonObject newProp;
        newProp["propertyId"] = propertyId;
        newProp["value"] = QJsonValue::fromVariant(value);
        props.append(newProp);
    }

    root["properties"] = props;
    page.content = QJsonDocument(root).toJson(QJsonDocument::Compact);

    if (m_PageManager) {
        m_PageManager->updatePageContent(page.id, page.content);
    }
    
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

QVariant DatabaseModel::getProperty(int row, int propertyId) const {
    if (row < 0 || row >= m_childPages.count()) return QVariant();
    return getPropertyValue(m_childPages[row], propertyId);
}

QVariant DatabaseModel::getPropertyByPageId(int pageId, int propertyId) const {
    for (const auto &page : m_childPages) {
        if (page.id == pageId) return getPropertyValue(page, propertyId);
    }
    return QVariant();
}

void DatabaseModel::setPropertyByPageId(int pageId, int propertyId, const QVariant &value) {
    for (int i = 0; i < m_childPages.count(); ++i) {
        if (m_childPages[i].id == pageId) {
            setProperty(i, propertyId, value);
            return;
        }
    }
}

int DatabaseModel::getPropertyIdByName(const QString &name) const {
    for (const auto &s : m_schema) {
        if (s.name.compare(name, Qt::CaseInsensitive) == 0) return s.id;
    }
    return -1;
}
