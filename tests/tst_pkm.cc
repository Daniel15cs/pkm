#include <QtTest>
#include <qobject.h>
#include <qtestcase.h>
#include <qtmetamacros.h>
#include "PageModel.h"

class TestPkm: public QObject{
	Q_OBJECT
	
private slots:
void test_true(){
	QVERIFY(true);
}
void test_pageModel(){
	PageModel *pm = new PageModel();
	PageData pd{2,1,"note",""};
	pm->setPageData(&pd);
	pm->append("checkboxBlock");

	QCOMPARE(*pm->getPageData(), pd);
}
};

QTEST_MAIN(TestPkm)
#include <tst_pkm.moc>
