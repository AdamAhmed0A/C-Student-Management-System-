#include "collegecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../database/persistence.h"

CollegeController::CollegeController() {}

bool CollegeController::addCollege(const College& college)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("INSERT INTO colleges (name, code, tuition_fees) VALUES (?, ?, ?)");
    query.addBindValue(college.name());
    query.addBindValue(college.code());
    query.addBindValue(college.tuitionFees());
    if (query.exec()) {
        Persistence::logChange("Faculty", "Create", query.lastInsertId().toInt(), college.name());
        return true;
    }
    return false;
}

bool CollegeController::updateCollege(const College& college)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("UPDATE colleges SET name = ?, code = ?, tuition_fees = ? WHERE id = ?");
    query.addBindValue(college.name());
    query.addBindValue(college.code());
    query.addBindValue(college.tuitionFees());
    query.addBindValue(college.id());
    if (query.exec()) {
        Persistence::logChange("Faculty", "Edit", college.id(), college.name());
        return true;
    }
    return false;
}

bool CollegeController::deleteCollege(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_COLLEGE);
    query.addBindValue(id);
    if (query.exec()) {
        Persistence::logChange("Faculty", "Delete", id, "Removed Faculty");
        return true;
    }
    return false;
}

QList<College> CollegeController::getAllColleges()
{
    QList<College> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_COLLEGES)) {
        while (query.next()) {
            list.append(College(query.value("id").toInt(),
                              query.value("name").toString(),
                              query.value("code").toString(),
                              query.value("tuition_fees").toDouble()));
        }
    }
    return list;
}
