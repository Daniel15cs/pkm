#include <QtTest>
#include <cstdarg>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qtestcase.h>
#include "../src/filemodel.h"

class TestFileModel: public QObject{
	Q_OBJECT
	private:
	// QSqlDatabase db;
	FileModel *fm;
	QVector<PageData> list ;

private slots:
	void init(){
		list = {PageData{1,-1, "note", "01.01.2026", "bla bla bla"}};
		fm = new FileModel();
		fm->openDb(":memory:");

		QSqlQuery q(fm->db);
		QString query ="insert into Notes(id,parentId,type,content,created_at)"
		"values(1,-1,\"note\",\"bla bla bla\", \"01.01.2026\")";
		QVERIFY(q.exec(query));
	}

	void cleanup(){
		fm->db.close();
	}
		// QVERIFY(true);
		// QCOMPARE(1,1);

	// void tst_simple_updateDb(){
	// 	// PageData pd{2,1,"note","content"};
	// 	QVector<PageData> l;
	// 	l.append(pd);
	//
	// 	QVERIFY(fm->updateListToDb(l));
	// }
	void updateDb(int id,PageData *pd){
		PageData expected = *pd;
		QVector<PageData> l;
		l.append(expected);
		QVERIFY(fm->updateListToDb(l));
		PageData actual = fm->getPageDataFromSql(id);
		qDebug()<<fm->getPageContentFromSql(id);
		QCOMPARE(fm->getPageContentFromSql(id).toUtf8(),expected.content);
		QCOMPARE(actual.type,expected.type);
		QCOMPARE(actual.parentId,expected.parentId);
		QCOMPARE(actual.id,expected.id);
	}
	void tst_updateItem_updateDb(){
		int id = 1;
		PageData expected{id,1,"note","content bla erwerwer bla 123456"};
		updateDb(id,&expected);
	}
	void tst_insertItem_updateDb(){
		int id = 2;
		PageData expected{id,-1,"note","bbalsdff content bla bla bla "};
		updateDb(id,&expected);
	}



	void tst_getContent(){
		QCOMPARE(fm->getPageContentFromSql(list[0].id), list[0].content);
	}

	void tst_getPageData(){
		PageData pd = fm->getPageDataFromSql(list[0].id);
		QCOMPARE(pd.content,list[0].content);
		QCOMPARE(pd.type,list[0].type);
		QCOMPARE(pd.parentId,list[0].parentId);
		QCOMPARE(pd.id,list[0].id);
	}

	void tst_list(){
		QVector<PageData> list= {PageData{1,-1,"note",""}, PageData{2,1,"","content {bla bla bla}"}};
		QCOMPARE(list[1], list[1]);
	}

	void tst_firstPD_getPagesList(){
		QVector<PageData> l = fm->getPagesListFromSql();
		l[0].content = fm->getPageContentFromSql(l[0].id).toUtf8();
		QCOMPARE(l[0],list[0]);
	}

	void tst_firstProps_getPagesList(){
		QVector<PageData> l = fm->getPagesListFromSql();
		l[0].content = fm->getPageContentFromSql(l[0].id).toUtf8();
		QCOMPARE(l[0].id,list[0].id);
		QCOMPARE(l[0].content,list[0].content);
		QCOMPARE(l[0].type,list[0].type);
		QCOMPARE(l[0].parentId,list[0].parentId);

	}
	void tst_append_updateListToDb(){
		int id = 3;
		list.append(PageData{id,1,"note","content"});

		PageData pd = fm->getPageDataFromSql(id);

		// QVERIFY(fm->updateListToDb(list));
		// QCOMPARE(fm->getPagesListFromSql(),list);
		//
		// list[2].parentId = 2;
		// QVERIFY(fm->updateListToDb(list));
		// QCOMPARE(fm->getPagesListFromSql(),list);

	}
	// void test_pageModel(){
	// }
};

QTEST_MAIN(TestFileModel)
#include <tst_fileModel.moc>
