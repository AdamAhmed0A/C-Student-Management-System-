#include "academic_level_controller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AcademicLevelController::AcademicLevelController() {}

bool AcademicLevelController::addAcademicLevel(const AcademicLevel& level)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_ACADEMIC_LEVEL);
    query.addBindValue(level.name());
    query.addBindValue(level.levelNumber());
    return query.exec();
}

bool AcademicLevelController::updateAcademicLevel(const AcademicLevel& level)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_ACADEMIC_LEVEL);
    query.addBindValue(level.name());
    query.addBindValue(level.levelNumber());
    query.addBindValue(level.id());
    return query.exec();
}

bool AcademicLevelController::deleteAcademicLevel(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_ACADEMIC_LEVEL);
    query.addBindValue(id);
    return query.exec();
}

QList<AcademicLevel> AcademicLevelController::getAllAcademicLevels()
{
    QList<AcademicLevel> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_ACADEMIC_LEVELS)) {
        while (query.next()) {
            list.append(AcademicLevel(query.value("id").toInt(),
                                    query.value("name").toString(),
                                    query.value("level_number").toInt()));
        }
    }
    return list;
}
