#include "calendar_event.h"

/**
 * Default Constructor
 * Initializes a new instance with default ID 0
 */
CalendarEvent::CalendarEvent()
    : m_id(0)
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the event
 * @param title - The title of the event
 * @param description - Detailed description of the event
 * @param startDate - The start date of the event
 * @param endDate - The end date of the event
 * @param eventType - The type/category of the event
 */
CalendarEvent::CalendarEvent(int id, const QString& title, const QString& description, const QDate& startDate, const QDate& endDate, const QString& eventType)
    : m_id(id), m_title(title), m_description(description), m_startDate(startDate), m_endDate(endDate), m_eventType(eventType)
{
}

/**
 * @return The unique ID of the calendar event
 */
int CalendarEvent::id() const { return m_id; }

/**
 * Sets the unique ID of the calendar event
 * @param id - The new ID
 */
void CalendarEvent::setId(int id) { m_id = id; }

/**
 * @return The title of the event
 */
QString CalendarEvent::title() const { return m_title; }

/**
 * Sets the title of the event
 * @param title - The new title
 */
void CalendarEvent::setTitle(const QString& title) { m_title = title; }

/**
 * @return The description of the event
 */
QString CalendarEvent::description() const { return m_description; }

/**
 * Sets the description of the event
 * @param description - The new description
 */
void CalendarEvent::setDescription(const QString& description) { m_description = description; }

/**
 * @return The start date of the event
 */
QDate CalendarEvent::startDate() const { return m_startDate; }

/**
 * Sets the start date of the event
 * @param startDate - The new start date
 */
void CalendarEvent::setStartDate(const QDate& startDate) { m_startDate = startDate; }

/**
 * @return The end date of the event
 */
QDate CalendarEvent::endDate() const { return m_endDate; }

/**
 * Sets the end date of the event
 * @param endDate - The new end date
 */
void CalendarEvent::setEndDate(const QDate& endDate) { m_endDate = endDate; }

/**
 * @return The type or category of the event
 */
QString CalendarEvent::eventType() const { return m_eventType; }

/**
 * Sets the type or category of the event
 * @param eventType - The new event type
 */
void CalendarEvent::setEventType(const QString& eventType) { m_eventType = eventType; }
