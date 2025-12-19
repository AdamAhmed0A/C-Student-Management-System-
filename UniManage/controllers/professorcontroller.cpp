#include "professorcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ProfessorController::ProfessorController() {}

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

bool ProfessorController::deleteProfessor(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_PROFESSOR);
    query.addBindValue(id);
    return query.exec();
}

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
