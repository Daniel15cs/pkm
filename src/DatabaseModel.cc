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
            if (role == Qt::DisplayRole && m_PageManager) return m_PageManager->getPageTitle(page.id);
            if (!page.title.isEmpty()) return page.title;
            return QString("Unnamed"); // Fallback
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
    m_childPages.clear();
    m_schema.clear();

    parseSchema(dbContent);
    loadVisibility();

    if (m_PageManager) {
        m_childPages = m_PageManager->getPagesByParent(m_parentId);
    }
    endResetModel();
    emit schemaChanged();
}

void DatabaseModel::refresh() {
    if (m_PageManager) {
        beginResetModel();
        Page p = m_PageManager->getPageById(m_parentId);
        parseSchema(p.data.content);
        loadVisibility();
        m_childPages = m_PageManager->getPagesByParent(m_parentId);
        endResetModel();
        emit schemaChanged();
    }
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
        map["visibleTable"] = isPropertyVisible(s.id, "table");
        map["visibleKanban"] = isPropertyVisible(s.id, "kanban");
        map["visibleList"] = isPropertyVisible(s.id, "list");
        list.append(map);
    }
    return list;
}

void DatabaseModel::parseSchema(const QByteArray &dbContent) {
    m_schema.clear();

    // Always add Creation Date as the first property
    PropertySchema createdAt;
    createdAt.id = 0; // Fixed ID for system property
    createdAt.name = "Created At";
    createdAt.type = "creation_date";
    m_schema.append(createdAt);

    QJsonDocument doc = QJsonDocument::fromJson(dbContent);
    if (!dbContent.isEmpty() && doc.isObject() && doc.object().contains("properties")) {
        QJsonArray props = doc.object()["properties"].toArray();
        for (const auto &val : props) {
            QJsonObject obj = val.toObject();
            PropertySchema ps;
            ps.id = obj["propertyId"].toInt();
            if (ps.id == 0) continue; // Skip if somehow a custom prop has ID 0

            ps.name = obj["name"].toString();
            if (ps.name.isEmpty()) ps.name = obj["type"].toString(); // Fallback
            ps.type = obj["type"].toString();
            
            QJsonArray vals = obj["values"].toArray();
            for (const auto &v : vals) ps.values.append(v.toString());
            
            m_schema.append(ps);
        }
    } else if (dbContent.isEmpty() || !doc.isObject()) {
        // Default schema for brand new databases (if no schema exists yet)
        PropertySchema ps;
        ps.id = 1;
        ps.name = "Status";
        ps.type = "status";
        ps.values = {"To-Do", "In-progress", "Done"};
        m_schema.append(ps);
    }

    emit schemaChanged();
}

QVariant DatabaseModel::getPropertyValue(const PageData &page, int propertyId) const {
    for (const auto &s : m_schema) {
        if (s.id == propertyId && s.type == "creation_date") {
            return page.createdAt;
        }
    }

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
    
    QJsonValue jsonVal;
    if (value.typeId() == QMetaType::QVariantList || value.typeId() == QMetaType::QStringList) {
        jsonVal = QJsonArray::fromVariantList(value.toList());
    } else {
        jsonVal = QJsonValue::fromVariant(value);
    }

    bool found = false;
    for (int i = 0; i < props.size(); ++i) {
        QJsonObject obj = props[i].toObject();
        if (obj["propertyId"].toInt() == propertyId) {
            obj["value"] = jsonVal;
            props[i] = obj;
            found = true;
            break;
        }
    }

    if (!found) {
        QJsonObject newProp;
        newProp["propertyId"] = propertyId;
        newProp["value"] = jsonVal;
        props.append(newProp);
    }

    root["properties"] = props;
    page.content = QJsonDocument(root).toJson(QJsonDocument::Compact);

    if (m_PageManager) {
        m_PageManager->updatePageContent(page.id, page.content);
    }
    
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    emit propertyChanged(page.id, propertyId, value);
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

bool DatabaseModel::isPropertyVisible(int propertyId, const QString &viewType) const {
    if (!m_visiblePropertiesCache.contains(viewType)) return true;
    return m_visiblePropertiesCache[viewType].contains(propertyId);
}

void DatabaseModel::setPropertyVisible(int propertyId, const QString &viewType, bool visible) {
    if (!m_PageManager) return;

    // Ensure cache is initialized for this viewType if it's the first time hiding something
    if (!m_visiblePropertiesCache.contains(viewType)) {
        QSet<int> allProps;
        for (const auto &s : m_schema) allProps.insert(s.id);
        m_visiblePropertiesCache[viewType] = allProps;
    }

    if (visible) {
        m_visiblePropertiesCache[viewType].insert(propertyId);
    } else {
        m_visiblePropertiesCache[viewType].remove(propertyId);
    }

    // Save back to JSON
    Page p = m_PageManager->getPageById(m_parentId);
    QJsonDocument doc = QJsonDocument::fromJson(p.data.content);
    QJsonObject root = doc.isObject() ? doc.object() : QJsonObject();
    QJsonObject viewConfig = root["viewConfig"].toObject();
    QJsonObject view = viewConfig[viewType].toObject();
    
    QJsonArray newVisibleProps;
    for (int id : m_visiblePropertiesCache[viewType]) {
        newVisibleProps.append(id);
    }
    
    view["visibleProperties"] = newVisibleProps;
    viewConfig[viewType] = view;
    root["viewConfig"] = viewConfig;
    
    m_PageManager->updatePageContent(m_parentId, QJsonDocument(root).toJson(QJsonDocument::Compact));
    emit schemaChanged(); // Trigger UI update
}

void DatabaseModel::loadVisibility() {
    m_visiblePropertiesCache.clear();
    if (!m_PageManager) return;

    Page p = m_PageManager->getPageById(m_parentId);
    QJsonDocument doc = QJsonDocument::fromJson(p.data.content);
    if (!doc.isObject()) return;

    QJsonObject root = doc.object();
    QJsonObject viewConfig = root["viewConfig"].toObject();
    QStringList viewTypes = {"table", "kanban", "list"};
    
    for (const QString &vt : viewTypes) {
        if (viewConfig.contains(vt)) {
            QJsonObject view = viewConfig[vt].toObject();
            if (view.contains("visibleProperties")) {
                QJsonArray vProps = view["visibleProperties"].toArray();
                QSet<int> propSet;
                for (const auto &v : vProps) propSet.insert(v.toInt());
                m_visiblePropertiesCache[vt] = propSet;
            }
        }
    }
}

void DatabaseModel::saveSchema() {
    if (!m_PageManager) return;

    Page p = m_PageManager->getPageById(m_parentId);
    QJsonDocument doc = QJsonDocument::fromJson(p.data.content);
    QJsonObject root = doc.isObject() ? doc.object() : QJsonObject();

    QJsonArray props;
    for (const auto &s : m_schema) {
        if (s.id == 0) continue; // Don't save system properties

        QJsonObject obj;
        obj["propertyId"] = s.id;
        obj["name"] = s.name;
        obj["type"] = s.type;
        
        QJsonArray vals;
        for (const auto &v : s.values) vals.append(v);
        obj["values"] = vals;
        
        props.append(obj);
    }

    root["properties"] = props;
    m_PageManager->updatePageContent(m_parentId, QJsonDocument(root).toJson(QJsonDocument::Compact));
}

void DatabaseModel::addProperty(const QString &name, const QString &type) {
    int maxId = 0;
    for (const auto &s : m_schema) {
        if (s.id > maxId) maxId = s.id;
    }

    PropertySchema ps;
    ps.id = maxId + 1;
    ps.name = name;
    ps.type = type;
    if (type == "status") {
        ps.values = {"To-Do", "In-progress", "Done"};
    }

    m_schema.append(ps);
    saveSchema();
    emit schemaChanged();
    beginResetModel(); endResetModel(); // Force column count update
}

void DatabaseModel::removeProperty(int propertyId) {
    if (propertyId == 0) return; // Cannot remove system property

    for (int i = 0; i < m_schema.count(); ++i) {
        if (m_schema[i].id == propertyId) {
            m_schema.remove(i);
            saveSchema();
            emit schemaChanged();
            beginResetModel(); endResetModel();
            return;
        }
    }
}

void DatabaseModel::renameProperty(int propertyId, const QString &newName) {
    if (propertyId == 0) return; // Cannot rename system property

    for (auto &s : m_schema) {
        if (s.id == propertyId) {
            s.name = newName;
            saveSchema();
            emit schemaChanged();
            return;
        }
    }
}

void DatabaseModel::addPropertyValue(int propertyId, const QString &value) {
    for (auto &s : m_schema) {
        if (s.id == propertyId) {
            if (!s.values.contains(value)) {
                s.values.append(value);
                saveSchema();
                emit schemaChanged();
            }
            return;
        }
    }
}
