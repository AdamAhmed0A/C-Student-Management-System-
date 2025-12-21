#include "calendar_event.h"

CalendarEvent::CalendarEvent()
    : m_id(0)
{
}

CalendarEvent::CalendarEvent(int id, const QString& title, const QString& description, const QDate& startDate, const QDate& endDate, const QString& eventType)
    : m_id(id), m_title(title), m_description(description), m_startDate(startDate), m_endDate(endDate), m_eventType(eventType)
{
}

int CalendarEvent::id() const { return m_id; }
void CalendarEvent::setId(int id) { m_id = id; }

QString CalendarEvent::title() const { return m_title; }
void CalendarEvent::setTitle(const QString& title) { m_title = title; }

QString CalendarEvent::description() const { return m_description; }
void CalendarEvent::setDescription(const QString& description) { m_description = description; }

QDate CalendarEvent::startDate() const { return m_startDate; }
void CalendarEvent::setStartDate(const QDate& startDate) { m_startDate = startDate; }

QDate CalendarEvent::endDate() const { return m_endDate; }
void CalendarEvent::setEndDate(const QDate& endDate) { m_endDate = endDate; }

QString CalendarEvent::eventType() const { return m_eventType; }
void CalendarEvent::setEventType(const QString& eventType) { m_eventType = eventType; }
