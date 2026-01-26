#include "filemodel.h"
#include <QDebug>

FileModel::FileModel(QObject *parent)
    : QObject{parent}
{}
int FileModel::test(){
		qDebug()<<"my| test is success";
		return 100;
	}
