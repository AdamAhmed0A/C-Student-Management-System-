#include "departmentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DepartmentController::DepartmentController() {}

bool DepartmentController::addDepartment(const Department& dept)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_DEPARTMENT);
    query.addBindValue(dept.name());
    query.addBindValue(dept.collegeId());
    query.addBindValue(dept.code());
    return query.exec();
}

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

bool DepartmentController::deleteDepartment(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_DEPARTMENT);
    query.addBindValue(id);
    return query.exec();
}

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
