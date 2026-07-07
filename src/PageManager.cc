#include "PageManager.h"
#include "filemodel.h"
#include <filesystem>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qlogging.h>

#include "DatabaseModel.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <functional>

void PageManager::setFileModel(FileModel *fm){
	this->fileModel = fm;
}

int PageManager::appendPageToList(int parentId, QString type){
	int maxId = 0;
	if(fileModel){
		for(const auto& p : fileModel->getPagesListFromSql()) {
			if(p.id > maxId) maxId = p.id;
		}
	}
	for(const auto& p : pagesList) {
		if(p.data.id > maxId) maxId = p.data.id;
	}
	qDebug()<<"max id: "<<maxId;
	
	PageData *pd = new PageData();
	pd->id = maxId + 1;
	qDebug()<<"pdId: "<<pd->id;
	pd->parentId= parentId;
	pd->type = type;
	pd->createdAt = QDateTime::currentDateTime().toString("dd.MM.yyyy");
	pd->title = "Unnamed";

	PageModel  *pm =new PageModel(this);
	pm->setPageData(pd);
	pm->callback = [&](int parentId, QString type)->int{
		qDebug()<<"PManager::callback parentId: "<<parentId;
		return this->appendPageToList(parentId, type);
	};

	DatabaseModel *dbm = new DatabaseModel(this);
	dbm->setManager(this);
	dbm->load(pd->id, QByteArray());

	Page p = {*pd, pm, dbm};
	this->pagesList.append(p);
	emit pagesChanged();
	return pd->id;
}
void PageManager::uploadList(){ // parse from raw sqlite data to model
	qDebug()<<"uploadList";
	this->pagesList.clear();
	for(PageData item : fileModel->getPagesListByParentFromSql(0)){
		PageData *pd = new PageData(item);

		PageModel *pm = new PageModel(this);
		pm->setPageData(pd);
		pm->callback = [&](int parentId, QString type)->int{
			qDebug()<<"callback";
			return this->appendPageToList(parentId, type);
		};

		DatabaseModel *dbm = new DatabaseModel(this);
		dbm->setManager(this);

		QString contentStr = fileModel->getPageContentFromSql(item.id);
		pm->parseJson(contentStr);
        item.content = contentStr.toUtf8();

		if(item.type == "DataBase") {
			dbm->load(item.id, contentStr.toUtf8());
		}

		Page p = {item, pm, dbm};
		this->pagesList.append(p);
	} 
    // Also check for -1 if no 0s found, or just load both.
    if(pagesList.isEmpty()) {
        for(PageData item : fileModel->getPagesListByParentFromSql(-1)){
            PageData *pd = new PageData(item);
            PageModel *pm = new PageModel(this);
            pm->setPageData(pd);
            pm->callback = [&](int parentId, QString type)->int{ return this->appendPageToList(parentId, type); };
            DatabaseModel *dbm = new DatabaseModel(this);
            dbm->setManager(this);
            QString contentStr = fileModel->getPageContentFromSql(item.id);
            pm->parseJson(contentStr);
            item.content = contentStr.toUtf8();
            if(item.type == "DataBase") dbm->load(item.id, contentStr.toUtf8());
            Page p = {item, pm, dbm};
            this->pagesList.append(p);
        }
    }
    emit pagesChanged();
	if(!pagesList.isEmpty()) setCurrentPage(pagesList[0].data.id);
}

PageManager::PageManager(FileModel* fm){
	// fileModel->openDb();
	fileModel =fm;
	this->uploadList();
//debug for input raw data
	// for(auto item:pagesList){
	// 	auto d = item.data;
	// 	auto m = item.model;
	// 	qDebug()<<"data: id:"<<d.id<<": pId"<<d.parentId<<": type"<<d.type;
	// 	qDebug()<<"rowCount: "<< m->rowCount();
	// 	qDebug()<<"content: "<<fileModel->getPageContentFromSql(d.id);
	// }
	if(!pagesList.isEmpty())
		currentPage = pagesList[0];
	else{
		qDebug()<<"pageManager constr: pagesList is empty";
		appendPageToList();
		currentPage = pagesList[0];
	}
		// currentPage = Page();
		// pagesList.append(currentPage);
		// pagesList.append(Page());
		// currentPage = pagesList[0];
}
void PageManager::setCurrentPage(int id){
    if (currentPage.data.id != -1 && currentPage.data.id != id) {
        // Only push to history if it's not the same as the last entry
        if (historyStack.isEmpty() || historyStack.last() != currentPage.data.id) {
            historyStack.append(currentPage.data.id);
        }
    }

		lastPage = currentPage;
        bool found = false;
		for(auto item:pagesList) {
			if(item.data.id==id){
				currentPage=item;
				emit currPageChanged(currentPage);
                found = true;
                break;
			}
		}
		//BUG: after open nested page, blocks of new page did not appears, instead first block of old page was displayed
        if(!found) {
            // Load from DB
            PageData pd = fileModel->getPageDataFromSql(id);
            if(pd.id != -1) {
                PageModel *pm = new PageModel(this);
                pm->setPageData(new PageData(pd));
                pm->callback = [&](int parentId, QString type)->int{ return this->appendPageToList(parentId, type); };
                QString contentStr = fileModel->getPageContentFromSql(id);
                pm->parseJson(contentStr);
                DatabaseModel *dbm = new DatabaseModel(this);
                dbm->setManager(this);
                if(pd.type == "DataBase") dbm->load(id, contentStr.toUtf8());
                Page p = {pd, pm, dbm};
                pagesList.append(p);
                currentPage = p;
                emit currPageChanged(currentPage);
                emit pagesChanged();
            }
        }
}

Page PageManager::getCurrentPage(){
	for(auto item: pagesList){
		if(item.data.id==currentPage.data.id){
			// qDebug()<<"currpage:" <<item.data.id;
			return item;
		}
	}
	return currentPage;
}
void PageManager::getToLastPage(){
    if (!historyStack.isEmpty()) {
        int previousId = historyStack.takeLast();
        int targetId = previousId;
        for(auto item:pagesList) {
            if(item.data.id==targetId){
                currentPage=item;
                emit currPageChanged(currentPage);
                return;
            }
        }

        PageData pd = fileModel->getPageDataFromSql(targetId);
        if(pd.id != -1) {
            PageModel *pm = new PageModel(this);
            pm->setPageData(new PageData(pd));
            pm->callback = [&](int parentId, QString type)->int{ return this->appendPageToList(parentId, type); };
            QString contentStr = fileModel->getPageContentFromSql(targetId);
            pm->parseJson(contentStr);
            DatabaseModel *dbm = new DatabaseModel(this);
            dbm->setManager(this);
            if(pd.type == "DataBase") dbm->load(targetId, contentStr.toUtf8());
            Page p = {pd, pm, dbm};
            pagesList.append(p);
            currentPage = p;
            emit currPageChanged(currentPage);
            emit pagesChanged();
        }
    }
}
void PageManager::savePagesList(){
	QVector<PageData> pdList;
	qDebug() << "PageManager::savePagesList() saving" << pagesList.size() << "pages";
	for(auto &item : pagesList){
		PageData pd = item.data;
		pd.content = item.model->listToJson();
        item.data.content = pd.content; // Keep in-memory data in sync
		pdList.append(pd);
	}

	bool ok = fileModel->updateListToDb(pdList);
    qDebug() << "saveList result: " << ok;
    emit pagesChanged(); // Refresh sidebar titles
}

void PageManager::setPageTitle(int id, const QString &title) {
    for (int i = 0; i < pagesList.size(); ++i) {
        if (pagesList[i].data.id == id) {
            pagesList[i].data.title = title;
            if (fileModel) {
                fileModel->setDataToSql(pagesList[i].data, id);
            }
            emit pagesChanged();
            break;
        }
    }
}

Page PageManager::getPageById(int id){
	for(Page item:pagesList){
		if(item.data.id==id){
			return item;
		}
	}
    // Load from DB if not found?
	return Page();
}

QString PageManager::getPageTitle(int id) {
    QString title;
    QByteArray content;
		//TODO: this is a shit

    // Check in memory first
    bool found = false;
    for (const auto &p : pagesList) {
        if (p.data.id == id) {
            title = p.data.title;
            content = p.data.content;
            if (content.isEmpty() && p.model) content = p.model->listToJson();
            found = true;
            break;
        }
    }
    if (!found && fileModel) {
        // Check in DB
        PageData pd = fileModel->getPageDataFromSql(id);
        title = pd.title;
        content = pd.content;
    }

    return title.isEmpty() ? "Unnamed" : title;
}

QVector<PageData> PageManager::getPagesByParent(int parentId) {
    QVector<PageData> result;
    QSet<int> ids;
    for (const auto &p : pagesList) {
        if (p.data.parentId == parentId) {
            result.append(p.data);
            ids.insert(p.data.id);
        }
    }
    // Also fetch from DB for nested pages not yet in memory
    if (fileModel) {
        for (const auto &pd : fileModel->getPagesListByParentFromSql(parentId)) {
            if (!ids.contains(pd.id)) {
                result.append(pd);
            }
        }
    }
    return result;
}

void PageManager::updatePageContent(int pageId, const QByteArray &content) {
    for (int i = 0; i < pagesList.size(); ++i) {
        if (pagesList[i].data.id == pageId) {
            pagesList[i].data.content = content;

            if (fileModel) {
                fileModel->setDataToSql(pagesList[i].data, pageId);
            }
            break;
        }
    }
}

QVariantList PageManager::rootPages() {
    QVariantList roots;
    qDebug() << "PageManager::rootPages() called. pagesList size:" << pagesList.size();
    for (const auto &p : pagesList) {
        // Any page that has no parent (0 or -1) is considered a root page.
        if (p.data.parentId <= 0) {
            QVariantMap m;
            m["id"] = p.data.id;
            m["title"] = getPageTitle(p.data.id);
            roots.append(m);
            qDebug() << "Adding root page:" << m["id"] << m["title"];
        }
    }
    return roots;
}

static QString extractSearchableText(const QByteArray &content) {
    QString text;
    QJsonDocument doc = QJsonDocument::fromJson(content);
    if (!doc.isObject()) return QString();
    
    std::function<void(const QJsonArray&)> walk = [&](const QJsonArray &blocks) {
        for (const auto &v : blocks) {
            QJsonObject b = v.toObject();
            QJsonObject c = b["content"].toObject();
            if (c.contains("text")) text += c["text"].toString() + " ";
            if (b.contains("children")) walk(b["children"].toArray());
        }
    };
    
    walk(doc.object()["blockList"].toArray());
    QJsonArray props = doc.object()["properties"].toArray();
    for (const auto &v : props) text += v.toObject()["value"].toString() + " ";
    return text;
}

QVariantList PageManager::search(const QString &query) {
    QVariantList results;
    QSet<int> foundIds;
    QString queryLower = query.toLower();

    qDebug() << "PageManager::search() query:" << query;

    // 1. Search in open pages (memory)
    for (const auto &p : pagesList) {
        QByteArray content = p.data.content.isEmpty() ? p.model->listToJson() : p.data.content;
        QString searchable = extractSearchableText(content) + p.data.title;
        if (searchable.toLower().contains(queryLower)) {
            QVariantMap m;
            m["id"] = p.data.id;
            m["title"] = getPageTitle(p.data.id);
            results.append(m);
            foundIds.insert(p.data.id);
        }
    }

    // 2. Search in DB
    QVariantList dbResults = fileModel->searchInDb(query);
    for (const QVariant &v : dbResults) {
        QVariantMap m = v.toMap();
        int id = m["id"].toInt();
        if (!foundIds.contains(id)) {
            // Refine search to avoid JSON keys
            QByteArray content = fileModel->getPageContentFromSql(id).toUtf8();
            QString searchable = extractSearchableText(content) + m["title"].toString();
            if (searchable.toLower().contains(queryLower)) {
                m["title"] = getPageTitle(id);
                results.append(m);
                foundIds.insert(id);
            }
        }
    }

    return results;
}
