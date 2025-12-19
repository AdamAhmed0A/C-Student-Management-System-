#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "../models/student_data.h"
#include <QList>
#include <QString>

class StudentController
{
public:
    StudentController();
    
    bool addStudent(const StudentData& student);
    bool updateStudent(const StudentData& student);
    bool deleteStudent(int id);
    
    QList<StudentData> getAllStudents();
    StudentData getStudentById(int id);
    StudentData getStudentByUserId(int userId);
    StudentData getStudentByIdNumber(int IdNumber);
};

#endif // STUDENTCONTROLLER_H
