#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QString>
#include <QTime>

class Schedule
{
public:
    Schedule();
    
    int id() const;
    void setId(int id);
    
    int courseId() const;
    void setCourseId(int courseId);
    
    int roomId() const;
    void setRoomId(int roomId);
    
    int professorId() const;
    void setProfessorId(int professorId);
    
    QString dayOfWeek() const;
    void setDayOfWeek(const QString& day);
    
    QTime startTime() const;
    void setStartTime(const QTime& time);
    
    QTime endTime() const;
    void setEndTime(const QTime& time);
    
    // Helpers
    QString courseName() const;
    void setCourseName(const QString& name);
    
    QString roomName() const;
    void setRoomName(const QString& name);
    
    QString professorName() const;
    void setProfessorName(const QString& name);

private:
    int m_id;
    int m_courseId;
    int m_roomId;
    int m_professorId;
    QString m_dayOfWeek;
    QTime m_startTime;
    QTime m_endTime;
    
    QString m_courseName;
    QString m_roomName;
    QString m_professorName;
};

#endif // SCHEDULE_H
