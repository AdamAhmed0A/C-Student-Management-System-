#include "collegecontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../database/persistence.h"

/**
 * Constructor for the CollegeController class
 */
CollegeController::CollegeController() {}

/**
 * Adds a new college/faculty to the database
 * Logs the creation event after successful insertion
 * @param college - The College object containing the details
 * @return True if successful, otherwise false
 */
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

/**
 * Updates an existing college in the database
 * Logs the update event after successful execution
 * @param college - The College object with updated details
 * @return True if successful, otherwise false
 */
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

/**
 * Deletes a college from the database
 * Logs the deletion event after successful execution
 * @param id - The ID of the college to delete
 * @return True if successful, otherwise false
 */
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

/**
 * Retrieves all colleges from the database
 * @return A list of College objects
 */
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

/**
 * Retrieves a college by its ID
 * @param id - The ID of the college to retrieve
 * @return The College object if found, otherwise an empty object
 */
College CollegeController::getCollegeById(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("SELECT * FROM colleges WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return College(query.value("id").toInt(),
                      query.value("name").toString(),
                      query.value("code").toString(),
                      query.value("tuition_fees").toDouble());
    }
    return College();
}
