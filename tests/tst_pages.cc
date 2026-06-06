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
#include "../src/BlockSystem/bulletBlock.h"

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
        m.append("bulletBlock");

        QByteArray json = m.listToJson();

        PageModel m2;
        m2.parseJson(json);

        QCOMPARE(m2.rowCount(), 4);

        QCOMPARE(QString(m2.getLogic(0).value<QObject*>()->metaObject()->className()),
                 QString("TextBlock"));

        QCOMPARE(QString(m2.getLogic(1).value<QObject*>()->metaObject()->className()),
                 QString("CheckboxBlock"));
        QCOMPARE(QString(m2.getLogic(2).value<QObject*>()->metaObject()->className()),
                 QString("PageBlock"));
        QCOMPARE(QString(m2.getLogic(3).value<QObject*>()->metaObject()->className()),
                 QString("BulletBlock"));
    }

    void testToggleBlockRoundTrip() {
        PageModel m;
        m.append("toggleBlock");
        ToggleBlock* tg = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        tg->setText("Parent");
        tg->setExpanded(true);

        m.append("textBlock");
        Block* child = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        m.addIndentBlock(child); 
        static_cast<TextBlock*>(child)->setText("Child");

        QCOMPARE(m.rowCount(), 2);
        QCOMPARE(tg->children.count(), 1);

        QByteArray json = m.listToJson();
        
        PageModel m2;
        m2.parseJson(json);

        QCOMPARE(m2.rowCount(), 2);
        ToggleBlock* tg2 = qobject_cast<ToggleBlock*>(m2.getLogic(0).value<QObject*>());
        QVERIFY(tg2);
        QCOMPARE(tg2->text(), QString("Parent"));
        QCOMPARE(tg2->children.count(), 1);
        
        TextBlock* child2 = qobject_cast<TextBlock*>(tg2->children.at(0));
        QVERIFY(child2);
        QCOMPARE(child2->text(), QString("Child"));
    }

    void testToggleBlockCollapsedAfterParse() {
        PageModel m;
        m.append("toggleBlock");
        ToggleBlock* tg = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        tg->setText("Parent");
        tg->setExpanded(false);

        m.append("textBlock");
        Block* child = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        m.addIndentBlock(child); 

        QCOMPARE(m.rowCount(), 1); 
        QCOMPARE(tg->children.count(), 1);

        QByteArray json = m.listToJson();
        
        PageModel m2;
        m2.parseJson(json);

        QCOMPARE(m2.rowCount(), 1); 
        ToggleBlock* tg2 = qobject_cast<ToggleBlock*>(m2.getLogic(0).value<QObject*>());
        QCOMPARE(tg2->expanded(), false);
        QCOMPARE(tg2->children.count(), 1);
    }

    void testCountChilds() {
        PageModel m;
        m.append("toggleBlock"); // 0
        m.append("textBlock");   // 1
        m.append("textBlock");   // 2

        ToggleBlock* t = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        Block* b1 = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        Block* b2 = qobject_cast<Block*>(m.getLogic(2).value<QObject*>());

        m.addIndentBlock(b1);
        m.addIndentBlock(b2);

        QCOMPARE(m.countChilds(t), 2);
        
        t->setExpanded(false);
        QCOMPARE(m.countChilds(t), 0); // countChilds only counts if expanded
    }

    void testAddIndentBlock() {
        PageModel m;
        m.append("toggleBlock"); // A
        m.append("textBlock");   // B
        
        ToggleBlock* a = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        Block* b = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        
        m.addIndentBlock(b);
        QCOMPARE(b->parentBlock(), a);
        QVERIFY(a->children.contains(b));
        QCOMPARE(b->level(), 1);
    }

    void testRmIndentBlock() {
        PageModel m;
        m.append("toggleBlock"); // A
        m.append("textBlock");   // B
        
        ToggleBlock* a = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        Block* b = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        
        m.addIndentBlock(b);
        m.rmIndentBlock(b);
        
        QVERIFY(b->parentBlock() == nullptr);
        QVERIFY(!a->children.contains(b));
        QCOMPARE(b->level(), 0);
    }

    void testRmChildList() {
        PageModel m;
        m.append("toggleBlock"); // 0
        m.append("textBlock");   // 1
        
        ToggleBlock* t = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        Block* b = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        
        m.addIndentBlock(b);
        QCOMPARE(m.rowCount(), 2);
        
        m.rmChildList(t);
        QCOMPARE(m.rowCount(), 1);
        QCOMPARE(m.getLogic(0).value<QObject*>(), t);
    }

    void testInsertChildList() {
        PageModel m;
        m.append("toggleBlock");
        ToggleBlock* t = qobject_cast<ToggleBlock*>(m.getLogic(0).value<QObject*>());
        t->setExpanded(false);

        Block* child = new TextBlock(&m);
        t->children.append(child);
        child->setParentBlock(t);

        QCOMPARE(m.rowCount(), 1);
        m.toggle(t); // will call insertChildList
        QCOMPARE(m.rowCount(), 2);
        QCOMPARE(m.getLogic(1).value<QObject*>(), child);
    }

    void testNonToggleHierarchy() {
        PageModel m;
        m.append("textBlock"); // A
        m.append("textBlock"); // B
        
        Block* a = qobject_cast<Block*>(m.getLogic(0).value<QObject*>());
        Block* b = qobject_cast<Block*>(m.getLogic(1).value<QObject*>());
        
        m.addIndentBlock(b);
        
        QCOMPARE(b->parentBlock(), a);
        QVERIFY(a->children.contains(b));
        QCOMPARE(b->level(), 1);
        QCOMPARE(m.rowCount(), 2); // Should still be visible
        
        QByteArray json = m.listToJson();
        PageModel m2;
        m2.parseJson(json);
        
        QCOMPARE(m2.rowCount(), 2);
        Block* a2 = qobject_cast<Block*>(m2.getLogic(0).value<QObject*>());
        QCOMPARE(a2->children.count(), 1);
        QCOMPARE(a2->children.at(0)->level(), 1);
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
