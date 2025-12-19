#ifndef ENROLLMENTCONTROLLER_H
#define ENROLLMENTCONTROLLER_H

#include "../models/enrollment.h"
#include <QList>

class EnrollmentController
{
public:
    EnrollmentController();

    bool addEnrollment(const Enrollment& enrollment);
    bool updateEnrollment(const Enrollment& enrollment);
    bool deleteEnrollment(int id);

    QList<Enrollment> getEnrollmentsByStudent(int studentId);
};

#endif // ENROLLMENTCONTROLLER_H
