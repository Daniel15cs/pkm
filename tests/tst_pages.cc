#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QSqlQuery>

#include "PageModel.h"
#include "PageManager.h"
#include "filemodel.h"
// #include "../src/BlockSystem/block.h"
#include "../src/BlockSystem/checkboxBlock.h"
#include "../src/BlockSystem/textBlock.h"
#include "../src/BlockSystem/pageBlock.h"

// =======================================================
// TestPageModel
// =======================================================

class TestPageModel : public QObject {
    Q_OBJECT

private:
    FileModel* fileModel = nullptr;

private slots:

    void testAppendCreatesCorrectTypes()
    {
        PageModel m;

        m.append();                  // TextBlock
        m.append("checkboxBlock");   // CheckboxBlock

        QCOMPARE(m.rowCount(), 2);

        QObject* b0 = m.getLogic(0).value<QObject*>();
        QObject* b1 = m.getLogic(1).value<QObject*>();

        QVERIFY(b0);
        QVERIFY(b1);
				// QVERIFY(false);
        QCOMPARE(QString(b0->metaObject()->className()), QString("TextBlock"));
        QCOMPARE(QString(b1->metaObject()->className()), QString("CheckboxBlock"));
    }

    void testInsertMaintainsOrder()
    {
        PageModel m;

        m.append();
        m.append();

        m.insert("checkboxBlock", 1);

        QCOMPARE(m.rowCount(), 3);

        QObject* mid = m.getLogic(2).value<QObject*>();
        QCOMPARE(QString(mid->metaObject()->className()), QString("TextBlock")); // default insert logic
    }

    void testRemoveActuallyRemoves() {
        PageModel m;

        m.append();
        m.append();
        m.append();

        QObject* secondBefore = m.getLogic(1).value<QObject*>();
        m.removeRow(1);

        QCOMPARE(m.rowCount(), 2);

        QObject* newSecond = m.getLogic(1).value<QObject*>();
        QVERIFY(newSecond != secondBefore);
    }

    void testJsonRoundTripPreservesTypes() {
					
        PageModel m;

        m.append();
        m.append("checkboxBlock");
        m.append("pageBlock");

        QByteArray json = m.listToJson();

        PageModel m2;
        m2.parseJson(json);

        QCOMPARE(m2.rowCount(), 3);

        QCOMPARE(QString(m2.getLogic(0).value<QObject*>()->metaObject()->className()),
                 QString("TextBlock"));

        QCOMPARE(QString(m2.getLogic(1).value<QObject*>()->metaObject()->className()),
                 QString("CheckboxBlock"));
        QCOMPARE(QString(m2.getLogic(2).value<QObject*>()->metaObject()->className()),
                 QString("PageBlock"));
    }

    void testParseJsonRestoresData()
    {
        PageModel m;

        QByteArray json = R"(
        {
            "blockList": [
                {
                    "type": "textBlock",
                    "content": { "text": "hello" }
                },
                {
                    "type": "checkboxBlock",
                    "content": { "text": "ok", "state": 1 }
                }
            ]
        })";

        m.parseJson(json);

        QObject* t0 = m.getLogic(0).value<QObject*>();
        QObject* t1 = m.getLogic(1).value<QObject*>();

        auto tb = qobject_cast<TextBlock*>(t0);
        auto cb = qobject_cast<CheckboxBlock*>(t1);

        QVERIFY(tb);
        QVERIFY(cb);

        QCOMPARE(tb->text(), QString("hello"));
        QCOMPARE(cb->text(), QString("ok"));
        QCOMPARE(cb->checkState(), 1);
    }
// };


// =======================================================
// TestPageManager
// =======================================================

// class TestPageManager : public QObject
// {
//     Q_OBJECT
//
// private slots:

    void initTestCase()
    {
        fileModel = new FileModel();

        QVERIFY(fileModel->openDb(":memory:"));

        QSqlQuery q(fileModel->db);

        QVERIFY(q.exec(R"(
            CREATE TABLE Notes(
                id INTEGER PRIMARY KEY NOT NULL,
                parentId INTEGER NOT NULL DEFAULT -1,
                type TEXT NOT NULL DEFAULT "note",
                content TEXT
            )
        )"));

        QVERIFY(q.exec(R"(
            INSERT INTO Notes(id, parentId, type, content)
            VALUES (1, -1, "note", "bla bla bla")
        )"));
    }

    void cleanupTestCase()
    {
        delete fileModel;
        fileModel = nullptr;
    }

    void testAppendPageCreatesPage() {
			PageManager pm(fileModel);

			int newId = pm.appendPageToList(0);

			QVERIFY(newId > 0);

			// перевіряємо, що сторінка існує в менеджері
			Page p = pm.getPageById(newId);
			QVERIFY(p.model != nullptr);

			QCOMPARE(p.data.id, newId);
    }

    void testSetCurrentPageChangesState()
    {
        PageManager pm(fileModel);

        int p1 = pm.appendPageToList(0);
        int p2 = pm.appendPageToList(p1);

        QSignalSpy spy(&pm, SIGNAL(currPageChanged(Page)));

        pm.setCurrentPage(p2);

        QCOMPARE(spy.count(), 1);

        QCOMPARE(pm.getCurrentPage().data.id, p2);
    }

    void testGetToLastPageUsesParent()
    {
        PageManager pm(fileModel);

        int parent = pm.appendPageToList(0);
        int child  = pm.appendPageToList(parent);

        pm.setCurrentPage(child);
        pm.getToLastPage();

        QCOMPARE(pm.getCurrentPage().data.id, parent);
    }

    void testPageBlockCallbackCreatesNewPage()
    {
			PageManager pm(fileModel);

			pm.appendPageToList(0);

			PageModel* model = pm.getCurrentPage().model;

			int rowsBefore = model->rowCount();

			model->append("pageBlock");

			QCOMPARE(model->rowCount(), rowsBefore + 1);

			auto obj = model->getLogic(rowsBefore).value<QObject*>();

			auto pb = qobject_cast<PageBlock*>(obj);

			QVERIFY(pb);
			QVERIFY(pb->getPageId() > 0);
    }

    void testSavePagesListUpdatesDb()
    {
        PageManager pm(fileModel);

        pm.appendPageToList(0);
        pm.appendPageToList(0);

        pm.savePagesList();

        // перевірка що хоча б щось записалось
        QSqlQuery q(fileModel->db);
        QVERIFY(q.exec("SELECT COUNT(*) FROM Notes"));

        QVERIFY(q.next());
        int count = q.value(0).toInt();

        QVERIFY(count >= 1);
    }
};


// =======================================================
// MAIN
// =======================================================

QTEST_MAIN(TestPageModel)
#include "tst_pages.moc"
