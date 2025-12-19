#ifndef COLLEGECONTROLLER_H
#define COLLEGECONTROLLER_H

#include "../models/college.h"
#include <QList>

class CollegeController
{
public:
    CollegeController();
    bool addCollege(const College& college);
    bool updateCollege(const College& college);
    bool deleteCollege(int id);
    QList<College> getAllColleges();
};

#endif // COLLEGECONTROLLER_H
