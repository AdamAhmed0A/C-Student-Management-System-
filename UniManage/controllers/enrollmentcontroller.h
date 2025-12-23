#ifndef ENROLLMENTCONTROLLER_H
#define ENROLLMENTCONTROLLER_H

#include "../models/enrollment.h"
#include "../models/attendance_log.h"
#include <QList>
#include <QString>
#include <QDate> // Required for QDate parameter in new function

class EnrollmentController
{
public:
    EnrollmentController();

    bool addEnrollment(const Enrollment& e);
    bool updateEnrollment(const Enrollment& e);
    bool deleteEnrollment(int id);

    QList<Enrollment> getEnrollmentsByStudent(int studentId);
    QList<Enrollment> getEnrollmentsByCourse(int courseId);
    Enrollment getEnrollmentById(int id);

    void calculateTotalAndGrade(Enrollment& e, const QString& courseType, int maxMarks);
    
    // Attendance Logs
    bool addAttendanceLog(const AttendanceLog& log);
    QList<AttendanceLog> getAttendanceLogsByCourse(int courseId, const QDate& date);
};

#endif // ENROLLMENTCONTROLLER_H
