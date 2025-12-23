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

bool ScheduleController::assignProfessorToCourse(int courseId, int professorId)
{
    QSqlDatabase& db = DBConnection::instance().database();
    
    // Check if a schedule entry already exists for this course
    QSqlQuery check(db);
    check.prepare("SELECT id FROM schedules WHERE course_id = ? LIMIT 1");
    check.addBindValue(courseId);
    
    if (check.exec() && check.next()) {
        // Update existing entry
        QSqlQuery update(db);
        update.prepare("UPDATE schedules SET professor_id = ? WHERE course_id = ?");
        update.addBindValue(professorId);
        update.addBindValue(courseId);
        return update.exec();
    } else {
        // Insert new entry with default values for day/time/room
        // We need a roomId. We'll pick the first room or leave it 0 if DB allows (FK might fail if 0)
        // Let's find a valid room id
        int roomId = 1; // Default
        QSqlQuery roomQuery("SELECT id FROM rooms LIMIT 1", db);
        if (roomQuery.exec() && roomQuery.next()) {
            roomId = roomQuery.value(0).toInt();
        }
        
        QSqlQuery insert(db);
        insert.prepare(Queries::INSERT_SCHEDULE);
        insert.addBindValue(courseId);
        insert.addBindValue(roomId);
        insert.addBindValue(professorId);
        insert.addBindValue("Monday");
        insert.addBindValue("09:00:00");
        insert.addBindValue("11:00:00");
        return insert.exec();
    }
}

bool ScheduleController::addSchedule(const Schedule& s)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_SCHEDULE);
    query.addBindValue(s.courseId());
    query.addBindValue(s.roomId());
    query.addBindValue(s.professorId());
    query.addBindValue(s.dayOfWeek());
    query.addBindValue(s.startTime().toString("HH:mm:ss"));
    query.addBindValue(s.endTime().toString("HH:mm:ss"));
    return query.exec();
}

bool ScheduleController::updateSchedule(const Schedule& s)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_SCHEDULE);
    query.addBindValue(s.courseId());
    query.addBindValue(s.roomId());
    query.addBindValue(s.professorId());
    query.addBindValue(s.dayOfWeek());
    query.addBindValue(s.startTime().toString("HH:mm:ss"));
    query.addBindValue(s.endTime().toString("HH:mm:ss"));
    query.addBindValue(s.id());
    return query.exec();
}

bool ScheduleController::deleteSchedule(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_SCHEDULE);
    query.addBindValue(id);
    return query.exec();
}

QList<Schedule> ScheduleController::getAllSchedules()
{
    QList<Schedule> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_SCHEDULES)) {
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
            s.setProfessorName(query.value("professor_name").toString());
            list.append(s);
        }
    }
    return list;
}

QList<Schedule> ScheduleController::getScheduleByCourse(int courseId)
{
    QList<Schedule> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_SCHEDULE_BY_COURSE);
    query.addBindValue(courseId);
    
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
    }
    return list;
}

QList<Schedule> ScheduleController::getScheduleByLevel(int levelId)
{
    QList<Schedule> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_SCHEDULE_BY_LEVEL);
    query.addBindValue(levelId);
    
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
    }
    return list;
}
