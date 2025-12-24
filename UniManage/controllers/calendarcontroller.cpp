#include "calendarcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * Constructor for the CalendarController class
 */
CalendarController::CalendarController() {}

/**
 * Adds a new calendar event to the database
 * @param event - The CalendarEvent object containing the event details
 * @return True if the operation was successful, otherwise false
 */
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

/**
 * Updates an existing calendar event in the database
 * @param event - The CalendarEvent object with updated details
 * @return True if the operation was successful, otherwise false
 */
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

/**
 * Deletes a calendar event from the database
 * @param id - The ID of the event to delete
 * @return True if the operation was successful, otherwise false
 */
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

/**
 * Retrieves all calendar events from the database
 * @return A list of CalendarEvent objects
 */
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

/**
 * Retrieves calendar events falling within a specific date range
 * @param start - The start date of the range
 * @param end - The end date of the range
 * @return A list of CalendarEvent objects within the specified range
 */
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
