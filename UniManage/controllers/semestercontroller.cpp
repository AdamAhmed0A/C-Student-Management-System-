#include "semestercontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SemesterController::SemesterController() {}

//function to add semester
bool SemesterController::addSemester(const Semester& semester)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_SEMESTER);
    query.addBindValue(semester.year());
    query.addBindValue(semester.semester());
    if (!query.exec()) {
        qDebug() << "Error adding semester:" << query.lastError().text();
        return false;
	}
	return true;
}

//function to update semester
bool SemesterController::updateSemester(const Semester& semester)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_SEMESTER);
    query.addBindValue(semester.year());
    query.addBindValue(semester.semester());
    query.addBindValue(semester.id());
    if (!query.exec()) {
        qDebug() << "Error updating semester:" << query.lastError().text();
        return false;
	}
	return true;
}

//function to delete semester
bool SemesterController::deleteSemester(int id)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_SEMESTER);
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error deleting semester:" << query.lastError().text();
        return false;
	}
    return true;
}

//function to get all semesters
QList<Semester> SemesterController::getAllSemesters()
{
	QList<Semester> semesters;
    QSqlQuery query(DBConnection::instance().database());
    if (!query.exec(Queries::SELECT_ALL_SEMESTERS)) {
        qDebug() << "Error retrieving semesters:" << query.lastError().text();
        return semesters;
    }
    
    while (query.next()) {
        Semester semester;
        semester.setId(query.value("id").toInt());
        semester.setYear(query.value("year").toDateTime());
        semester.setSemester(query.value("semester").toInt());
        semester.setCreatedAt(query.value("created_at").toDateTime());
        semester.setUpdatedAt(query.value("updated_at").toDateTime());
        
        semesters.append(semester);
	}
	return semesters;
}
