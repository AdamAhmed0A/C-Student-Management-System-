#ifndef STUDENT_DAO_H
#define STUDENT_DAO_H

#include "../UniManage/models/student_data.h"
#include <QList>
#include <QString>

class StudentDAO
{
public:
    virtual ~StudentDAO() = default;

    virtual bool insert(StudentData& student) = 0;
    virtual bool update(const StudentData& student) = 0;
    virtual bool remove(int id) = 0;
    virtual bool softDelete(int id) = 0;
    virtual bool restore(int id) = 0;
    virtual QList<StudentData> getAll() = 0;
    virtual QList<StudentData> getDeleted() = 0;
    virtual StudentData getById(int id) = 0;
    virtual StudentData getByUserId(int userId) = 0;
    virtual StudentData getByIdNumber(const QString& idNumber) = 0;
};

#endif // STUDENT_DAO_H