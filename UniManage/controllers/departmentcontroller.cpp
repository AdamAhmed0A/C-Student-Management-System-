#include "departmentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * Constructor for the DepartmentController class
 */
DepartmentController::DepartmentController() {}

/**
 * Adds a new department to the database
 * @param dept - The Department object containing details
 * @return True if successful, otherwise false
 */
bool DepartmentController::addDepartment(const Department& dept)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_DEPARTMENT);
    query.addBindValue(dept.name());
    query.addBindValue(dept.collegeId());
    query.addBindValue(dept.code());
    return query.exec();
}

/**
 * Updates an existing department in the database
 * @param dept - The Department object with updated details
 * @return True if successful, otherwise false
 */
bool DepartmentController::updateDepartment(const Department& dept)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_DEPARTMENT);
    query.addBindValue(dept.name());
    query.addBindValue(dept.collegeId());
    query.addBindValue(dept.code());
    query.addBindValue(dept.id());
    return query.exec();
}

/**
 * Deletes a department from the database
 * @param id - The ID of the department to delete
 * @return True if successful, otherwise false
 */
bool DepartmentController::deleteDepartment(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_DEPARTMENT);
    query.addBindValue(id);
    return query.exec();
}

/**
 * Retrieves all departments from the database
 * Also fetches the college name associated with each department
 * @return A list of Department objects
 */
QList<Department> DepartmentController::getAllDepartments()
{
    QList<Department> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_DEPARTMENTS)) {
        while (query.next()) {
            Department d(query.value("id").toInt(),
                        query.value("name").toString(),
                        query.value("college_id").toInt(),
                        query.value("code").toString());
            d.setCollegeName(query.value("college_name").toString());
            list.append(d);
        }
    }
    return list;
}

/**
 * Retrieves a department by its ID
 * @param id - The ID of the department to retrieve
 * @return The Department object if found, otherwise an empty object
 */
Department DepartmentController::getDepartmentById(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_DEPARTMENT_BY_ID);
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return Department(query.value("id").toInt(),
                         query.value("name").toString(),
                         query.value("college_id").toInt(),
                         query.value("code").toString());
    }
    return Department();
}

/**
 * Retrieves all departments associated with a specific college
 * @param collegeId - The ID of the college
 * @return A list of Department objects for that college
 */
QList<Department> DepartmentController::getDepartmentsByCollege(int collegeId)
{
    QList<Department> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_DEPARTMENTS_BY_COLLEGE);
    query.addBindValue(collegeId);
    if (query.exec()) {
        while (query.next()) {
            list.append(Department(query.value("id").toInt(),
                                 query.value("name").toString(),
                                 query.value("college_id").toInt(),
                                 query.value("code").toString()));
        }
    }
    return list;
}
