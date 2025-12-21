#ifndef SCHEDULECONTROLLER_H
#define SCHEDULECONTROLLER_H

#include "../models/schedule.h"
#include <QList>

class ScheduleController
{
public:
    ScheduleController();
    
    QList<Schedule> getScheduleByProfessor(int professorId);
    bool assignProfessorToCourse(int courseId, int professorId);
};

#endif // SCHEDULECONTROLLER_H
