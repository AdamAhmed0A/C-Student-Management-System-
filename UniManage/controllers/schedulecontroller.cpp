#include "schedulecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTime>

ScheduleController::ScheduleController() {}

QList<Schedule> ScheduleController::getScheduleByProfessor(int professorId)
{
    QList<Schedule> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_SCHEDULE_BY_PROFESSOR);
    query.addBindValue(professorId);
    
    if (query.exec()) {
        while (query.next()) {
            Schedule s;
            s.setId(query.value("id").toInt());
            s.setCourseId(query.value("course_id").toInt());
            s.setRoomId(query.value("room_id").toInt());
            s.setProfessorId(query.value("professor_id").toInt());
            s.setDayOfWeek(query.value("day_of_week").toString());
            s.setStartTime(query.value("start_time").toTime());
            s.setEndTime(query.value("end_time").toTime());
            s.setCourseName(query.value("course_name").toString());
            s.setRoomName(query.value("room_name").toString());
            list.append(s);
        }
    } else {
        qDebug() << "getScheduleByProfessor error:" << query.lastError().text();
    }
    return list;
}
