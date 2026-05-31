#include <QtTest>

class TestPages: public QObject{
	Q_OBJECT
private slots:
	void test_true(){
		QVERIFY(true);
	}
	void test_pageModel(){
		QCOMPARE(1,1);
	}
};

QTEST_MAIN(TestPages)
#include <tst_pages.moc>
