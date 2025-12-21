#ifndef ATTENDANCE_LOG_H
#define ATTENDANCE_LOG_H

#include <QString>
#include <QDate>
#include <QDateTime>

class AttendanceLog
{
public:
    AttendanceLog();
    
    int id() const;
    void setId(int id);
    
    int enrollmentId() const;
    void setEnrollmentId(int enrollmentId);
    
    QDate date() const;
    void setDate(const QDate& date);
    
    QString status() const;
    void setStatus(const QString& status);
    
    QString notes() const;
    void setNotes(const QString& notes);
    
private:
    int m_id;
    int m_enrollmentId;
    QDate m_date;
    QString m_status;
    QString m_notes;
};

#endif // ATTENDANCE_LOG_H
