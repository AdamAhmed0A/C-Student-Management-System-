#include "coursecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include "../database/persistence.h"

/**
 * Constructor for the CourseController class
 */
CourseController::CourseController() {}

/**
 * Adds a new course to the database
 * Logs the creation event after successful insertion
 * @param course - The Course object containing the details
 * @return True if successful, otherwise false
 */
bool CourseController::addCourse(const Course& course)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::INSERT_COURSE);
    query.addBindValue(course.name());
    query.addBindValue(course.description());
    query.addBindValue(course.yearLevel());
    query.addBindValue(course.creditHours());
    query.addBindValue(course.semesterId());
    query.addBindValue(course.departmentId());
    query.addBindValue(course.maxGrade());
    query.addBindValue(course.courseType());

    if (!query.exec()) {
        qDebug() << "addCourse failed:" << query.lastError().text();
        return false;
    }
    Persistence::logChange("Course", "Create", query.lastInsertId().toInt(), course.name());
    return true;
}

/**
 * Updates an existing course in the database
 * Logs the update event after successful execution
 * @param course - The Course object with updated details
 * @return True if successful, otherwise false
 */
bool CourseController::updateCourse(const Course& course)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::UPDATE_COURSE);
    query.addBindValue(course.name());
    query.addBindValue(course.description());
    query.addBindValue(course.yearLevel());
    query.addBindValue(course.creditHours());
    query.addBindValue(course.semesterId());
    query.addBindValue(course.departmentId());
    query.addBindValue(course.maxGrade());
    query.addBindValue(course.courseType());
    query.addBindValue(course.id());

    if (!query.exec()) {
        qDebug() << "updateCourse failed:" << query.lastError().text();
        return false;
    }
    Persistence::logChange("Course", "Edit", course.id(), course.name());
    return true;
}

/**
 * Deletes a course from the database
 * Also removes associated schedules, sections, and enrollments to maintain integrity
 * Logs the deletion event
 * @param id - The ID of the course to delete
 * @return True if successful, otherwise false
 */
bool CourseController::deleteCourse(int id)
{
    QSqlDatabase& db = DBConnection::instance().database();
    
    // 1. Delete Schedules
    QSqlQuery delSch(db);
    delSch.prepare("DELETE FROM schedules WHERE course_id = ?");
    delSch.addBindValue(id);
    delSch.exec();
    
    // 2. Delete Sections (Groups)
    QSqlQuery delSec(db);
    delSec.prepare("DELETE FROM sections WHERE course_id = ?");
    delSec.addBindValue(id);
    delSec.exec();
    
    // 3. Delete Enrollments (and logs via DB constraint or manual if needed, assuming simple structure for now)
    // Note: Enrollments might have foreign keys in attendance_logs. 
    // Ideally we should delete logs too, but let's try deleting enrollments directly.
    // If constraints fail, we'd need to delete from attendance_logs first.
    // Let's be safe:
    QSqlQuery delLogs(db);
    delLogs.prepare("DELETE FROM attendance_logs WHERE enrollment_id IN (SELECT id FROM enrollments WHERE course_id = ?)");
    delLogs.addBindValue(id);
    delLogs.exec();
    
    QSqlQuery delEnr(db);
    delEnr.prepare("DELETE FROM enrollments WHERE course_id = ?");
    delEnr.addBindValue(id);
    delEnr.exec();

    // 4. Finally Delete Course
    QSqlQuery query(db);
    query.prepare(Queries::DELETE_COURSE);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "deleteCourse failed:" << query.lastError().text();
        return false;
    }
    Persistence::logChange("Course", "Delete", id, "ID removed from system");
    return true;
}

/**
 * Retrieves all courses from the database
 * Populates Course objects with detailed information including semester, department, and professor assignments
 * @return A list of Course objects
 */
QList<Course> CourseController::getAllCourses()
{
    QList<Course> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);

    if (!query.exec(Queries::SELECT_ALL_COURSES)) {
        qDebug() << "getAllCourses failed:" << query.lastError().text();
        // Fallback: try simple query
        if (!query.exec("SELECT * FROM courses ORDER BY name")) {
            return list;
        }
    }

    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString description = query.value("description").toString();
        int yearLevel = query.value("year_level").toInt();
        int creditHours = query.value("credit_hours").toInt();
        int semesterId = query.value("semester_id").toInt();
        int maxGrade = query.value("max_grade").toInt();
        QString type = query.value("course_type").toString();

        Course c(id, name, description, yearLevel, creditHours, semesterId);
        c.setMaxGrade(maxGrade);
        c.setCourseType(type);

        QString createdStr = query.value("created_at").toString();
        if (!createdStr.isEmpty()) {
            QDateTime dt = QDateTime::fromString(createdStr, Qt::ISODate);
            if (dt.isValid()) c.setCreatedAt(dt);
        }

        QString updatedStr = query.value("updated_at").toString();
        if (!updatedStr.isEmpty()) {
            QDateTime dt = QDateTime::fromString(updatedStr, Qt::ISODate);
            if (dt.isValid()) c.setUpdatedAt(dt);
        }

        QString semesterYear = query.value("semester_year").toString();
        QString semesterNumber = query.value("semester_number").toString();
        
        c.setSemesterYear(semesterYear);
        c.setSemesterNumber(semesterNumber.toInt());
        
        if (!semesterYear.isEmpty() || !semesterNumber.isEmpty()) {
            QString semName = QString("%1 - Sem %2").arg(semesterYear).arg(semesterNumber);
            c.setSemesterName(semName);
        }

        c.setAssignedProfessor(query.value("assigned_professors").toString());
        c.setDepartmentId(query.value("department_id").toInt());
        c.setDepartmentName(query.value("department_name").toString());

        list.append(c);
    }
    return list;
}

/**
 * Retrieves courses associated with a specific semester
 * @param semesterId - The ID of the semester
 * @return A list of Course objects for that semester
 */
QList<Course> CourseController::getCoursesBySemester(int semesterId)
{
    QList<Course> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_COURSES_BY_SEMESTER);
    query.addBindValue(semesterId);

    if (!query.exec()) {
        qDebug() << "getCoursesBySemester failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString description = query.value("description").toString();
        int yearLevel = query.value("year_level").toInt();
        int creditHours = query.value("credit_hours").toInt();
        int semId = query.value("semester_id").toInt();
        int maxGrade = query.value("max_grade").toInt();
        QString type = query.value("course_type").toString();

        Course c(id, name, description, yearLevel, creditHours, semId);
        c.setMaxGrade(maxGrade);
        c.setCourseType(type);

        QString createdStr = query.value("created_at").toString();
        if (!createdStr.isEmpty()) {
            QDateTime dt = QDateTime::fromString(createdStr, Qt::ISODate);
            if (dt.isValid()) c.setCreatedAt(dt);
        }

        QString updatedStr = query.value("updated_at").toString();
        if (!updatedStr.isEmpty()) {
            QDateTime dt = QDateTime::fromString(updatedStr, Qt::ISODate);
            if (dt.isValid()) c.setUpdatedAt(dt);
        }

        list.append(c);
    }
    return list;
}

/**
 * Retrieves a course by its ID
 * @param id - The ID of the course to retrieve
 * @return The Course object with full details if found, otherwise an empty object
 */
Course CourseController::getCourseById(int id)
{
    Course c;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_COURSE_BY_ID);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "getCourseById failed:" << query.lastError().text();
        return c;
    }

    if (query.next()) {
        c.setId(query.value("id").toInt());
        c.setName(query.value("name").toString());
        c.setDescription(query.value("description").toString());
        c.setYearLevel(query.value("year_level").toInt());
        c.setCreditHours(query.value("credit_hours").toInt());
        c.setSemesterId(query.value("semester_id").toInt());
        c.setMaxGrade(query.value("max_grade").toInt());
        c.setCourseType(query.value("course_type").toString());

        QString createdStr = query.value("created_at").toString();
        if (!createdStr.isEmpty()) {
            QDateTime dt = QDateTime::fromString(createdStr, Qt::ISODate);
            if (dt.isValid()) c.setCreatedAt(dt);
        }

        QString updatedStr = query.value("updated_at").toString();
        if (!updatedStr.isEmpty()) {
            QDateTime dt = QDateTime::fromString(updatedStr, Qt::ISODate);
            if (dt.isValid()) c.setUpdatedAt(dt);
        }

        QString semesterYear = query.value("semester_year").toString();
        QString semesterNumber = query.value("semester_number").toString();
        c.setSemesterYear(semesterYear);
        c.setSemesterNumber(semesterNumber.toInt());
        
        if (!semesterYear.isEmpty() || !semesterNumber.isEmpty()) {
            QString semName = QString("%1 - Sem %2").arg(semesterYear).arg(semesterNumber);
            c.setSemesterName(semName);
        }
    }
    return c;
}

/**
 * Retrieves courses taught by a specific professor
 * @param professorId - The ID of the professor
 * @return A list of Course objects assigned to the professor
 */
QList<Course> CourseController::getCoursesByProfessor(int professorId)
{
    QList<Course> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_COURSES_BY_PROFESSOR);
    query.addBindValue(professorId);

    if (!query.exec()) {
        qDebug() << "getCoursesByProfessor failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString description = query.value("description").toString();
        int yearLevel = query.value("year_level").toInt();
        int creditHours = query.value("credit_hours").toInt();
        int semId = query.value("semester_id").toInt();
        int maxGrade = query.value("max_grade").toInt();
        QString type = query.value("course_type").toString();
        
        Course c(id, name, description, yearLevel, creditHours, semId);
        c.setMaxGrade(maxGrade);
        c.setCourseType(type);

        QString semesterYear = query.value("semester_year").toString();
        QString semesterNumber = query.value("semester_number").toString();
        c.setSemesterYear(semesterYear);
        c.setSemesterNumber(semesterNumber.toInt());
        
        if (!semesterYear.isEmpty() || !semesterNumber.isEmpty()) {
            QString semName = QString("%1 - Sem %2").arg(semesterYear).arg(semesterNumber);
            c.setSemesterName(semName);
        }

        list.append(c);
    }
    return list;
}
