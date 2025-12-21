#ifndef CALENDAR_EVENT_H
#define CALENDAR_EVENT_H

#include <QString>
#include <QDate>
#include <QDateTime>

class CalendarEvent
{
public:
    CalendarEvent();
    CalendarEvent(int id, const QString& title, const QString& description, const QDate& startDate, const QDate& endDate, const QString& eventType);

    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString& title);

    QString description() const;
    void setDescription(const QString& description);

    QDate startDate() const;
    void setStartDate(const QDate& startDate);

    QDate endDate() const;
    void setEndDate(const QDate& endDate);

    QString eventType() const;
    void setEventType(const QString& eventType);

private:
    int m_id;
    QString m_title;
    QString m_description;
    QDate m_startDate;
    QDate m_endDate;
    QString m_eventType;
};

#endif // CALENDAR_EVENT_H
