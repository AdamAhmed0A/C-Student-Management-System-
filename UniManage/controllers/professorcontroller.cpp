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
    return query.exec();
}

bool ProfessorController::updateProfessor(const Professor& prof)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_PROFESSOR);
    query.addBindValue(prof.specialization());
    query.addBindValue(prof.title());
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
            Professor p(query.value("id").toInt(),
                      query.value("user_id").toInt(),
                      query.value("specialization").toString(),
                      query.value("title").toString());
            p.setFullName(query.value("full_name").toString());
            list.append(p);
        }
    }
    return list;
}
