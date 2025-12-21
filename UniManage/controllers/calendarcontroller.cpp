#include "calendarcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CalendarController::CalendarController() {}

bool CalendarController::addEvent(const CalendarEvent& event)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_CALENDAR_EVENT);
    query.addBindValue(event.title());
    query.addBindValue(event.description());
    query.addBindValue(event.startDate());
    query.addBindValue(event.endDate());
    query.addBindValue(event.eventType());

    if (!query.exec()) {
        qDebug() << "addEvent failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CalendarController::updateEvent(const CalendarEvent& event)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_CALENDAR_EVENT);
    query.addBindValue(event.title());
    query.addBindValue(event.description());
    query.addBindValue(event.startDate());
    query.addBindValue(event.endDate());
    query.addBindValue(event.eventType());
    query.addBindValue(event.id());

    if (!query.exec()) {
        qDebug() << "updateEvent failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CalendarController::deleteEvent(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_CALENDAR_EVENT);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "deleteEvent failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<CalendarEvent> CalendarController::getAllEvents()
{
    QList<CalendarEvent> list;
    QSqlQuery query(DBConnection::instance().database());

    if (!query.exec(Queries::SELECT_ALL_CALENDAR_EVENTS)) {
        qDebug() << "getAllEvents failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        CalendarEvent e;
        e.setId(query.value("id").toInt());
        e.setTitle(query.value("title").toString());
        e.setDescription(query.value("description").toString());
        e.setStartDate(query.value("start_date").toDate());
        e.setEndDate(query.value("end_date").toDate());
        e.setEventType(query.value("event_type").toString());
        list.append(e);
    }
    return list;
}

QList<CalendarEvent> CalendarController::getEventsByRange(const QDate& start, const QDate& end)
{
    QList<CalendarEvent> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_CALENDAR_EVENTS_BY_RANGE);
    query.addBindValue(start);
    query.addBindValue(end);

    if (!query.exec()) {
        qDebug() << "getEventsByRange failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        CalendarEvent e;
        e.setId(query.value("id").toInt());
        e.setTitle(query.value("title").toString());
        e.setDescription(query.value("description").toString());
        e.setStartDate(query.value("start_date").toDate());
        e.setEndDate(query.value("end_date").toDate());
        e.setEventType(query.value("event_type").toString());
        list.append(e);
    }
    return list;
}
