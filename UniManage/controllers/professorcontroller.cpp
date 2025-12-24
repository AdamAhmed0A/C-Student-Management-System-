#include "professorcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * Constructor for the ProfessorController class
 */
ProfessorController::ProfessorController() {}

/**
 * Adds a new professor profile to the database
 * @param prof - The Professor object containing details
 * @return True if successful, otherwise false
 */
bool ProfessorController::addProfessor(const Professor& prof)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_PROFESSOR);
    query.addBindValue(prof.userId());
    query.addBindValue(prof.specialization());
    query.addBindValue(prof.title());
    query.addBindValue(prof.personalInfo());
    query.addBindValue(prof.idNumber());
    return query.exec();
}

/**
 * Updates an existing professor profile in the database
 * @param prof - The Professor object with updated details
 * @return True if successful, otherwise false
 */
bool ProfessorController::updateProfessor(const Professor& prof)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_PROFESSOR);
    query.addBindValue(prof.specialization());
    query.addBindValue(prof.title());
    query.addBindValue(prof.personalInfo());
    query.addBindValue(prof.idNumber());
    query.addBindValue(prof.id());
    return query.exec();
}

/**
 * Deletes a professor profile from the database
 * @param id - The ID of the professor to delete
 * @return True if successful, otherwise false
 */
bool ProfessorController::deleteProfessor(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_PROFESSOR);
    query.addBindValue(id);
    return query.exec();
}

/**
 * Retrieves all professors from the database
 * Joins with the users table to get full names
 * @return A list of Professor objects
 */
QList<Professor> ProfessorController::getAllProfessors()
{
    QList<Professor> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_PROFESSORS)) {
        while (query.next()) {
            Professor p;
            p.setId(query.value("id").toInt());
            p.setUserId(query.value("user_id").toInt());
            p.setIdNumber(query.value("id_number").toString());
            p.setSpecialization(query.value("specialization").toString());
            p.setTitle(query.value("title").toString());
            p.setPersonalInfo(query.value("personal_info").toString());
            p.setFullName(query.value("full_name").toString());
            list.append(p);
        }
    }
    return list;
}

/**
 * Retrieves a professor by their user ID
 * @param userId - The user ID from the users table
 * @return The Professor object if found, otherwise an empty object
 */
Professor ProfessorController::getProfessorByUserId(int userId)
{
    Professor p;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_PROFESSOR_BY_USER_ID);
    query.addBindValue(userId);
    if (query.exec() && query.next()) {
        p.setId(query.value("id").toInt());
        p.setUserId(query.value("user_id").toInt());
        p.setIdNumber(query.value("id_number").toString());
        p.setSpecialization(query.value("specialization").toString());
        p.setTitle(query.value("title").toString());
        p.setPersonalInfo(query.value("personal_info").toString());
        p.setFullName(query.value("full_name").toString());
    }
    return p;
}

/**
 * Retrieves a professor by their unique professor ID
 * @param id - The ID of the professor
 * @return The Professor object if found, otherwise an empty object
 */
Professor ProfessorController::getProfessorById(int id)
{
    Professor p;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("SELECT p.*, u.full_name FROM professors p JOIN users u ON p.user_id = u.id WHERE p.id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        p.setId(query.value("id").toInt());
        p.setUserId(query.value("user_id").toInt());
        p.setIdNumber(query.value("id_number").toString());
        p.setSpecialization(query.value("specialization").toString());
        p.setTitle(query.value("title").toString());
        p.setPersonalInfo(query.value("personal_info").toString());
        p.setFullName(query.value("full_name").toString());
    }
    return p;
}
