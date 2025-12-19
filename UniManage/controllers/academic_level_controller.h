#ifndef ACADEMICLEVELCONTROLLER_H
#define ACADEMICLEVELCONTROLLER_H

#include "../models/academic_level.h"
#include <QList>

class AcademicLevelController
{
public:
    AcademicLevelController();
    bool addAcademicLevel(const AcademicLevel& level);
    bool updateAcademicLevel(const AcademicLevel& level);
    bool deleteAcademicLevel(int id);
    QList<AcademicLevel> getAllAcademicLevels();
};

#endif // ACADEMICLEVELCONTROLLER_H
