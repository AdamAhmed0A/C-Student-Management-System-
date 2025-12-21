#ifndef CALENDARCONTROLLER_H
#define CALENDARCONTROLLER_H

#include "../models/calendar_event.h"
#include <QList>

class CalendarController
{
public:
    CalendarController();

    bool addEvent(const CalendarEvent& event);
    bool updateEvent(const CalendarEvent& event);
    bool deleteEvent(int id);
    QList<CalendarEvent> getAllEvents();
    QList<CalendarEvent> getEventsByRange(const QDate& start, const QDate& end);
};

#endif // CALENDARCONTROLLER_H
