#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "../models/student_data.h"
#include <QList>
#include <QString>

class StudentController
{
public:
    StudentController();
    
    bool addStudent(const StudentData& student, QString* error = nullptr);
    bool updateStudent(const StudentData& student);
    bool deleteStudent(int id);
    bool restoreStudent(int id);
    bool hardDeleteStudent(int id);
    
    QList<StudentData> getAllStudents();
    QList<StudentData> getDeletedStudents();
    StudentData getStudentById(int id);
    StudentData getStudentByUserId(int userId);
    StudentData getStudentByIdNumber(int IdNumber);
    
private:
    bool enrollStudentInLevelCourses(int studentId, int levelId);
};

#endif // STUDENTCONTROLLER_H
