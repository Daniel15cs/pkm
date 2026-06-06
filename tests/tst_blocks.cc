#include <QtTest>
#include <qjsonobject.h>
#include <qobject.h>
#include <qtestcase.h>
#include <qvariant.h>
#include "../src/BlockSystem/checkboxBlock.h"
#include "../src/BlockSystem/textBlock.h"
#include "../src/BlockSystem/pageBlock.h"
#include "../src/BlockSystem/toggleBlock.h"

class TestBlocks: public QObject{
	Q_OBJECT
private slots:
	void tst_textBlock_data(){
		QTest::addColumn<QString>("text");
		QTest::newRow("simple")<<"test text for test of textBlock";
		QTest::newRow("empty")<<"";
		// QTest::newRow("unicode")<<"перевірка тесту для тестування юнікоду ьії 日本語";

	}
	void tst_textBlock(){
		QFETCH(QString,text);
		Block * b;
		TextBlock *tb = new TextBlock(this);

		tb->setText(text);
		QCOMPARE(tb->text(), text);
		QCOMPARE(tb->getData(), text);
		b = tb;
		QCOMPARE(b->getData(),text);
		QCOMPARE(b->typeName(),"textBlock");


		//json parsing test
		QJsonObject con = b->blockToJson();
		QJsonObject res = con["content"].toObject();
		QCOMPARE(res["text"].toString(), text);
	}
	void tst_checkboxBlock_data(){
		tst_textBlock_data();
	}
	void tst_checkboxBlock(){
		QFETCH(QString,text);
		const QVariant state(2), state1(1);
		Block * b;
		CheckboxBlock *cb = new CheckboxBlock(this);

		cb->setState(state1);
		QCOMPARE(cb->checkState(),state1);

		cb->setState(state);
		cb->setText(text);
		b = cb;
		QCOMPARE(b->getData().toString(),text);
		QCOMPARE(b->typeName(),"checkboxBlock");

		//json parsing test
		QJsonObject obj = b->blockToJson();
		QJsonObject con = obj["content"].toObject();

		QString resText = con["text"].toString();
		QVariant resState = con["state"].toVariant();
		QCOMPARE(resText,text);
		QCOMPARE(resState,state);
	}
	void tst_pageBlock(){
		int num = 1;
		PageBlock *pb = new PageBlock(this);
		pb->setPageId(num);
		QCOMPARE(pb->getPageId(),num);
	}

	void tst_pageBlock_json(){
		int num = 1;
		PageBlock *pb = new PageBlock(this);
		pb->setPageId(num);
		QJsonObject obj = pb->blockToJson();

		QVERIFY(obj.contains("content"));
		QJsonObject con = obj["content"].toObject();

		QVERIFY(con.contains("id"));
		int id = con["id"].toInt();
		QCOMPARE(id,num);
	}

};

QTEST_MAIN(TestBlocks)
#include <tst_blocks.moc>
