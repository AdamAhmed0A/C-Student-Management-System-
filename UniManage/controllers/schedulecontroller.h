#ifndef SCHEDULECONTROLLER_H
#define SCHEDULECONTROLLER_H

#include "../models/schedule.h"
#include <QList>

class ScheduleController
{
public:
    ScheduleController();
    
    QList<Schedule> getScheduleByProfessor(int professorId);
    // Add other methods if needed (add, delete, etc.) but for Professor Panel we mainly need to View.
};

#endif // SCHEDULECONTROLLER_H
