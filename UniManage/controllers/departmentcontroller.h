#ifndef DEPARTMENTCONTROLLER_H
#define DEPARTMENTCONTROLLER_H

#include "../models/department.h"
#include <QList>

class DepartmentController
{
public:
    DepartmentController();
    bool addDepartment(const Department& department);
    bool updateDepartment(const Department& department);
    bool deleteDepartment(int id);
    QList<Department> getAllDepartments();
    QList<Department> getDepartmentsByCollege(int collegeId);
};

#endif // DEPARTMENTCONTROLLER_H
