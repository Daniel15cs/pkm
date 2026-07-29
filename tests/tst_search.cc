#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QSqlQuery>

#include "PageManager.h"
#include "filemodel.h"

class TestSearch : public QObject {
    Q_OBJECT

private:
    FileModel* fileModel = nullptr;

private slots:
    void initTestCase() {
        fileModel = new FileModel();
        QVERIFY(fileModel->openDb(":memory:"));

        QSqlQuery q(fileModel->db);
        // Page 1: Root page, contains "apple"
        QVERIFY(q.exec(R"(
            INSERT INTO Notes(title, id, parentId, type, content, created_at)
            VALUES ("apple", 1, 0, "note", '{"blockList":[{"type":"textBlock","content":{"text":"This is an apple"}},{"type":"textBlock","content":{"text":"Root page"}}]}', "01.01.2026")
        )"));
        // Page 2: Child of 1, contains "banana"
        QVERIFY(q.exec(R"(
            INSERT INTO Notes(title, id, parentId, type, content, created_at)
            VALUES ("banana",2, 1, "note", '{"blockList":[{"type":"textBlock","content":{"text":"This is a banana"}},{"type":"textBlock","content":{"text":"Child page"}}]}', "01.01.2026")
        )"));
        // Page 3: Root page, contains "cherry"
        QVERIFY(q.exec(R"(
            INSERT INTO Notes(title, id, parentId, type, content, created_at)
            VALUES ("cherry", 3, 0, "note", '{"blockList":[{"type":"textBlock","content":{"text":"This is a cherry"}},{"type":"textBlock","content":{"text":"Another root"}}]}', "01.01.2026")
        )"));
    }

    void cleanupTestCase() {
        delete fileModel;
        fileModel = nullptr;
    }

    void testRootPagesOnlyLoadedInitially() {
        PageManager pm(fileModel);
        // uploadList is called in constructor. It should only load root pages (parentId=0).
        QVariantList roots = pm.rootPages();
        QCOMPARE(roots.size(), 2); // IDs 1 and 3
        
        // Verify only 1 and 3 are in memory
        QCOMPARE(pm.getPageById(1).data.id, 1);
        QCOMPARE(pm.getPageById(3).data.id, 3);
        QCOMPARE(pm.getPageById(2).data.id, -1); // Should not be loaded yet
    }

    void testOnDemandLoading() {
        PageManager pm(fileModel);
        QCOMPARE(pm.getPageById(2).data.id, -1);
        
        pm.setCurrentPage(2);
        QCOMPARE(pm.getCurrentPage().data.id, 2);
        QCOMPARE(pm.getPageById(2).data.id, 2); // Now it should be loaded
    }

    void testSearchInMemoryFirst() {
        PageManager pm(fileModel);
        // Search for "apple" (Page 1 is in memory)
        QVariantList results = pm.search("apple");
        QCOMPARE(results.size(), 1);
        QCOMPARE(results[0].toMap()["id"].toInt(), 1);
    }

    void testSearchInDbAfterMemory() {
        PageManager pm(fileModel);
        // Search for "banana" (Page 2 is NOT in memory)
        QVariantList results = pm.search("banana");
        QCOMPARE(results.size(), 1);
        QCOMPARE(results[0].toMap()["id"].toInt(), 2);
        QCOMPARE(results[0].toMap()["title"].toString(), QString("banana"));
    }

    void testRootPagesProperty() {
        PageManager pm(fileModel);
        QVariantList roots = pm.rootPages();
        QCOMPARE(roots.size(), 2);
        
        QVariantMap r1 = roots[0].toMap();
        QVariantMap r2 = roots[1].toMap();
        
        // Note: Order might depend on DB insertion/loading
        bool found1 = (r1["id"].toInt() == 1 || r2["id"].toInt() == 1);
        bool found3 = (r1["id"].toInt() == 3 || r2["id"].toInt() == 3);
        
        QVERIFY(found1);
        QVERIFY(found3);
    }
};

QTEST_MAIN(TestSearch)
#include "tst_search.moc"
