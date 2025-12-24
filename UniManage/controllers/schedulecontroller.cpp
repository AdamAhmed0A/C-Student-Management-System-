#include "schedulecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTime>

/**
 * Constructor for the ScheduleController class
 */
ScheduleController::ScheduleController() {}

/**
 * Retrieves the schedule for a specific professor
 * @param professorId - The ID of the professor
 * @return A list of Schedule objects
 */
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

/**
 * Assigns a professor to a course, effectively creating or updating a schedule slot
 * If no schedule exists, it creates one with default timing and a default/virtual room
 * @param courseId - The ID of the course
 * @param professorId - The ID of the professor
 * @return True if successful, otherwise false
 */
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
        
        int roomId = 0;
        QSqlQuery roomQuery("SELECT id FROM rooms LIMIT 1", db);
        if (roomQuery.exec() && roomQuery.next()) {
            roomId = roomQuery.value(0).toInt();
        } else {
            // No rooms exist! Create a default one to allow assignment
            QSqlQuery createRoom(db);
            createRoom.prepare("INSERT INTO rooms (name, type, capacity, ac_units, fans_count, lighting_points, computers_count, seating_description, code) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
            createRoom.addBindValue("Virtual/TBA");
            createRoom.addBindValue("Hall");
            createRoom.addBindValue(100);
            createRoom.addBindValue(0);
            createRoom.addBindValue(0);
            createRoom.addBindValue(0);
            createRoom.addBindValue(0);
            createRoom.addBindValue("Standard");
            createRoom.addBindValue("TBA"); // Code
            
            if (createRoom.exec()) {
                roomId = createRoom.lastInsertId().toInt();
            } else {
                 qDebug() << "Failed to create default room:" << createRoom.lastError().text();
                 return false; 
            }
        }
        
        QSqlQuery insert(db);
        insert.prepare(Queries::INSERT_SCHEDULE);
        insert.addBindValue(courseId);
        insert.addBindValue(roomId);
        insert.addBindValue(professorId);
        insert.addBindValue("Monday");
        insert.addBindValue("09:00:00");
        insert.addBindValue("11:00:00");
        
        if (!insert.exec()) {
             qDebug() << "assignProfessorToCourse insert failed:" << insert.lastError().text();
             return false;
        }
        return true;
    }
}

/**
 * Adds a new schedule slot to the database
 * @param s - The Schedule object containing details
 * @return True if successful, otherwise false
 */
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

/**
 * Updates an existing schedule slot in the database
 * @param s - The Schedule object with updated details
 * @return True if successful, otherwise false
 */
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

/**
 * Deletes a schedule slot from the database
 * @param id - The ID of the schedule to delete
 * @return True if successful, otherwise false
 */
bool ScheduleController::deleteSchedule(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_SCHEDULE);
    query.addBindValue(id);
    return query.exec();
}

/**
 * Retrieves all schedule slots from the database
 * @return A list of Schedule objects
 */
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

/**
 * Retrieves schedule slots for a specific course
 * @param courseId - The ID of the course
 * @return A list of Schedule objects
 */
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

/**
 * Retrieves schedule slots for a specific academic level/year
 * @param levelId - The ID of the academic level
 * @return A list of Schedule objects
 */
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
