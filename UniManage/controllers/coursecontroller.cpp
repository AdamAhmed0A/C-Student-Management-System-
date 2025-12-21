#include "coursecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

CourseController::CourseController() {}

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
    query.addBindValue(course.maxGrade());
    query.addBindValue(course.courseType());

    if (!query.exec()) {
        qDebug() << "addCourse failed:" << query.lastError().text();
        return false;
    }
    return true;
}

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
    query.addBindValue(course.maxGrade());
    query.addBindValue(course.courseType());
    query.addBindValue(course.id());

    if (!query.exec()) {
        qDebug() << "updateCourse failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CourseController::deleteCourse(int id)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::DELETE_COURSE);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "deleteCourse failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<Course> CourseController::getAllCourses()
{
    QList<Course> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);

    if (!query.exec(Queries::SELECT_ALL_COURSES)) {
        qDebug() << "getAllCourses failed:" << query.lastError().text();
        return list;
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
        if (!semesterYear.isEmpty() || !semesterNumber.isEmpty()) {
            QString semName = QString("%1 - Sem %2").arg(semesterYear).arg(semesterNumber);
            c.setSemesterName(semName);
        }

        c.setAssignedProfessor(query.value("assigned_professors").toString());

        list.append(c);
    }
    return list;
}

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
        if (!semesterYear.isEmpty() || !semesterNumber.isEmpty()) {
            QString semName = QString("%1 - Sem %2").arg(semesterYear).arg(semesterNumber);
            c.setSemesterName(semName);
        }
    }
    return c;
}

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
        if (!semesterYear.isEmpty() || !semesterNumber.isEmpty()) {
            QString semName = QString("%1 - Sem %2").arg(semesterYear).arg(semesterNumber);
            c.setSemesterName(semName);
        }

        list.append(c);
    }
    return list;
}
