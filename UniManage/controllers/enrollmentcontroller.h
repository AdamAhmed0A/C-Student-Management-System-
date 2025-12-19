#ifndef ENROLLMENTCONTROLLER_H
#define ENROLLMENTCONTROLLER_H

#include "../models/enrollment.h"
#include <QList>
#include <QString>

class EnrollmentController
{
public:
    EnrollmentController();

    bool addEnrollment(const Enrollment& enrollment);
    bool updateEnrollment(const Enrollment& enrollment);
    bool deleteEnrollment(int id);

    QList<Enrollment> getEnrollmentsByStudent(int studentId);
    QList<Enrollment> getEnrollmentsByCourse(int courseId);

    void calculateTotalAndGrade(Enrollment& e, const QString& courseType, int maxMarks);
};

#endif // ENROLLMENTCONTROLLER_H
