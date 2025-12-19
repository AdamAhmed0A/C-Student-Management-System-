#include "collegecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CollegeController::CollegeController() {}

bool CollegeController::addCollege(const College& college)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_COLLEGE);
    query.addBindValue(college.name());
    query.addBindValue(college.code());
    return query.exec();
}

bool CollegeController::updateCollege(const College& college)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_COLLEGE);
    query.addBindValue(college.name());
    query.addBindValue(college.code());
    query.addBindValue(college.id());
    return query.exec();
}

bool CollegeController::deleteCollege(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_COLLEGE);
    query.addBindValue(id);
    return query.exec();
}

QList<College> CollegeController::getAllColleges()
{
    QList<College> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_COLLEGES)) {
        while (query.next()) {
            list.append(College(query.value("id").toInt(),
                              query.value("name").toString(),
                              query.value("code").toString()));
        }
    }
    return list;
}
