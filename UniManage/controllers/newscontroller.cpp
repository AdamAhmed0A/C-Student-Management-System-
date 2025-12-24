#include "newscontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

/**
 * Constructor for the NewsController class
 */
NewsController::NewsController() {}

/**
 * Adds a new news/announcement item to the database
 * @param news - The News object containing the content
 * @return True if successful, otherwise false
 */
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

/**
 * Updates an existing news item in the database
 * @param news - The News object with updated content
 * @return True if successful, otherwise false
 */
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

/**
 * Deletes a news item from the database
 * @param id - The ID of the news item to delete
 * @return True if successful, otherwise false
 */
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

/**
 * Retrieves all news items from the database
 * Orders them by date descending (implied by query usually)
 * @return A list of News objects
 */
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
