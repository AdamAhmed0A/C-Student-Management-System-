#include "newscontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

NewsController::NewsController() {}

bool NewsController::addNews(const News& news)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::INSERT_NEWS);
    query.addBindValue(news.title());
    query.addBindValue(news.body());

    if (!query.exec()) {
        qDebug() << "addNews failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool NewsController::updateNews(const News& news)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::UPDATE_NEWS);
    query.addBindValue(news.title());
    query.addBindValue(news.body());
    query.addBindValue(news.id());

    if (!query.exec()) {
        qDebug() << "updateNews failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool NewsController::deleteNews(int id)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::DELETE_NEWS);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "deleteNews failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<News> NewsController::getAllNews()
{
    QList<News> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);

    if (!query.exec(Queries::SELECT_ALL_NEWS)) {
        qDebug() << "getAllNews failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        News n;
        n.setId(query.value("id").toInt());
        n.setTitle(query.value("title").toString());
        n.setBody(query.value("body").toString());

        QDateTime created = query.value("created_at").toDateTime();
        if (created.isValid()) n.setCreatedAt(created);

        QDateTime updated = query.value("updated_at").toDateTime();
        if (updated.isValid()) n.setUpdatedAt(updated);

        list.append(n);
    }

    return list;
}
