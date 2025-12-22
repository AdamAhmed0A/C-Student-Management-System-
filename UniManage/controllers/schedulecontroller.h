#ifndef SCHEDULECONTROLLER_H
#define SCHEDULECONTROLLER_H

#include "../models/schedule.h"
#include <QList>

class ScheduleController
{
public:
    ScheduleController();
    
    QList<Schedule> getScheduleByProfessor(int professorId);
    bool addSchedule(const Schedule& s);
    bool updateSchedule(const Schedule& s);
    bool deleteSchedule(int id);
    QList<Schedule> getAllSchedules();
    bool assignProfessorToCourse(int courseId, int professorId);
};

#endif // SCHEDULECONTROLLER_H
