#include "DatabaseSortProxyModel.h"
#include "DatabaseModel.h"
#include <QDateTime>

DatabaseSortProxyModel::DatabaseSortProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
    setDynamicSortFilter(true);
}

void DatabaseSortProxyModel::setFilterStatus(const QString &status) {
    if (m_filterStatus != status) {
        m_filterStatus = status;
        invalidateFilter();
        emit filterStatusChanged();
    }
}

void DatabaseSortProxyModel::setFilterPropertyId(int propertyId) {
    m_filterPropertyId = propertyId;
    invalidateFilter();
}

void DatabaseSortProxyModel::sortByColumn(int column, Qt::SortOrder order) {
    sort(column, order);
}

QVariant DatabaseSortProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (sourceModel()) return sourceModel()->headerData(section, orientation, role);
    return QVariant();
}

bool DatabaseSortProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    if (m_filterStatus.isEmpty() || m_filterPropertyId == -1)
        return true;

    DatabaseModel *model = qobject_cast<DatabaseModel*>(sourceModel());
    if (!model) return true;

    QVariant val = model->getProperty(source_row, m_filterPropertyId);
    return val.toString() == m_filterStatus;
}

bool DatabaseSortProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {
    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);

    // Try date comparison first
    QDateTime leftDate = QDateTime::fromString(leftData.toString(), "dd.MM.yyyy HH:mm");
    QDateTime rightDate = QDateTime::fromString(rightData.toString(), "dd.MM.yyyy HH:mm");

    if (leftDate.isValid() && rightDate.isValid()) {
        return leftDate < rightDate;
    }

    // Fallback to numeric if possible
    bool okL, okR;
    double leftNum = leftData.toDouble(&okL);
    double rightNum = rightData.toDouble(&okR);
    if (okL && okR) return leftNum < rightNum;

    // Default to string comparison
    return leftData.toString().localeAwareCompare(rightData.toString()) < 0;
}
