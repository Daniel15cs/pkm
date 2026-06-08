#include <QtTest>
#include <QSignalSpy>
#include "DatabaseModel.h"
#include "DatabaseSortProxyModel.h"
#include "filemodel.h"
#include "PageManager.h"

class TestDatabase : public QObject {
    Q_OBJECT

private slots:
    void testSchemaParsing() {
        DatabaseModel model;
        QByteArray schemaJson = R"({
            "properties": [
                {"propertyId": 1, "name": "Status", "type": "status"},
                {"propertyId": 2, "name": "Priority", "type": "enum", "values": ["High", "Low"]}
            ]
        })";
        
        model.load(-1, schemaJson);
        
        QCOMPARE(model.columnCount(), 3); // Title + 2 properties
        QCOMPARE(model.headerData(1, Qt::Horizontal).toString(), QString("Status"));
        QCOMPARE(model.headerData(2, Qt::Horizontal).toString(), QString("Priority"));
    }

    void testPropertyAccess() {
        FileModel fm;
        fm.openDb(":memory:");
        PageManager manager(&fm);
        
        DatabaseModel model;
        model.setManager(&manager);
        
        QByteArray schemaJson = R"({"properties": [{"propertyId": 1, "name": "Status", "type": "status"}]})";
        
        // Add a child page through manager
        int childId = manager.appendPageToList(1, "note");
        QByteArray childContent = R"({"blockList":[], "properties": [{"propertyId": 1, "value": "in-progress"}]})";
        manager.updatePageContent(childId, childContent);
        
        model.load(1, schemaJson);
        
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.getProperty(0, 1).toString(), QString("in-progress"));
        
        // Test direct page ID access
        QCOMPARE(model.getPropertyByPageId(childId, 1).toString(), QString("in-progress"));
        model.setPropertyByPageId(childId, 1, "done");
        QCOMPARE(model.getPropertyByPageId(childId, 1).toString(), QString("done"));
    }

    void testSorting() {
        FileModel fm;
        fm.openDb(":memory:");
        PageManager manager(&fm);
        DatabaseModel model;
        model.setManager(&manager);
        
        QByteArray schema = R"({"properties": [{"propertyId": 1, "name": "Date", "type": "date"}]})";
        
        int id1 = manager.appendPageToList(10, "note");
        manager.updatePageContent(id1, R"({"properties":[{"propertyId":1,"value":"10.06.2026"}]})");
        
        int id2 = manager.appendPageToList(10, "note");
        manager.updatePageContent(id2, R"({"properties":[{"propertyId":1,"value":"01.06.2026"}]})");
        
        model.load(10, schema);
        
        DatabaseSortProxyModel proxy;
        proxy.setSourceModel(&model);
        
        proxy.sort(1, Qt::AscendingOrder);
        QCOMPARE(proxy.data(proxy.index(0, 1)).toString(), QString("01.06.2026"));
        
        proxy.sort(1, Qt::DescendingOrder);
        QCOMPARE(proxy.data(proxy.index(0, 1)).toString(), QString("10.06.2026"));
    }

    void testFiltering() {
        FileModel fm;
        fm.openDb(":memory:");
        PageManager manager(&fm);
        DatabaseModel model;
        model.setManager(&manager);
        
        QByteArray schema = R"({"properties": [{"propertyId": 1, "name": "Status", "type": "status"}]})";
        
        int id1 = manager.appendPageToList(10, "note");
        manager.updatePageContent(id1, R"({"properties":[{"propertyId":1,"value":"todo"}]})");
        
        int id2 = manager.appendPageToList(10, "note");
        manager.updatePageContent(id2, R"({"properties":[{"propertyId":1,"value":"done"}]})");
        
        model.load(10, schema);
        
        DatabaseSortProxyModel proxy;
        proxy.setSourceModel(&model);
        proxy.setFilterPropertyId(1);
        
        proxy.setFilterStatus("todo");
        QCOMPARE(proxy.rowCount(), 1);
        QCOMPARE(proxy.data(proxy.index(0, 1)).toString(), QString("todo"));
        
        proxy.setFilterStatus("done");
        QCOMPARE(proxy.rowCount(), 1);
        QCOMPARE(proxy.data(proxy.index(0, 1)).toString(), QString("done"));
        
        proxy.setFilterStatus("missing");
        QCOMPARE(proxy.rowCount(), 0);
    }
};

QTEST_MAIN(TestDatabase)
#include "tst_database.moc"
