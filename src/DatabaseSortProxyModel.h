#ifndef DATABASESORTPROXYMODEL_H
#define DATABASESORTPROXYMODEL_H

#include <QSortFilterProxyModel>

class DatabaseSortProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
    Q_PROPERTY(QString filterStatus READ filterStatus WRITE setFilterStatus NOTIFY filterStatusChanged)

public:
    explicit DatabaseSortProxyModel(QObject *parent = nullptr);

    QString filterStatus() const { return m_filterStatus; }
    void setFilterStatus(const QString &status);

    Q_INVOKABLE void sortByColumn(int column, Qt::SortOrder order);
    Q_INVOKABLE void setFilterPropertyId(int propertyId);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

signals:
    void filterStatusChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    QString m_filterStatus;
    int m_filterPropertyId = -1;
};

#endif // DATABASESORTPROXYMODEL_H
