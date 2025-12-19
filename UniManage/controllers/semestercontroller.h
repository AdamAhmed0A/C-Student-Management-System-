#ifndef SEMESTERCONTROLLER_H
#define SEMESTERCONTROLLER_H

#include "../models/semester.h"
#include <QList>

class SemesterController
{
public:
    SemesterController();
    
    bool addSemester(const Semester& semester);
    bool updateSemester(const Semester& semester);
    bool deleteSemester(int id);
    
    QList<Semester> getAllSemesters();
};

#endif // SEMESTERCONTROLLER_H
