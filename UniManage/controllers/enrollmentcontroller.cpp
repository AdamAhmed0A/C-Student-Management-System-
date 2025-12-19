#include "enrollmentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

EnrollmentController::EnrollmentController() {}

bool EnrollmentController::addEnrollment(const Enrollment& enrollment)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::INSERT_ENROLLMENT);
    query.addBindValue(enrollment.studentId());
    query.addBindValue(enrollment.status());
    query.addBindValue(enrollment.assessmentsGrades());
    query.addBindValue(enrollment.assignment1());
    query.addBindValue(enrollment.assignment2());
    query.addBindValue(enrollment.finalGrade());
    query.addBindValue(enrollment.totalMarks());
    query.addBindValue(enrollment.isRafaaApplied() ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "addEnrollment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool EnrollmentController::updateEnrollment(const Enrollment& enrollment)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::UPDATE_ENROLLMENT);
    query.addBindValue(enrollment.status());
    query.addBindValue(enrollment.assessmentsGrades());
    query.addBindValue(enrollment.assignment1());
    query.addBindValue(enrollment.assignment2());
    query.addBindValue(enrollment.finalGrade());
    query.addBindValue(enrollment.totalMarks());
    query.addBindValue(enrollment.isRafaaApplied() ? 1 : 0);
    query.addBindValue(enrollment.id());

    if (!query.exec()) {
        qDebug() << "updateEnrollment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool EnrollmentController::deleteEnrollment(int id)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::DELETE_ENROLLMENT);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "deleteEnrollment unsuccess:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<Enrollment> EnrollmentController::getEnrollmentsByStudent(int studentId)
{
    QList<Enrollment> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_ENROLLMENTS_BY_STUDENT);
    query.addBindValue(studentId);

    if (!query.exec()) {
        qDebug() << "getEnrollmentsByStudent unsucces:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        Enrollment eno;
        eno.setId(query.value("id").toInt());
        eno.setStudentId(query.value("student_id").toInt());
        eno.setStatus(query.value("status").toString());
        eno.setAssessmentsGrades(query.value("assessments_grades").toString());
        eno.setAssignment1(query.value("Assignment_1").toString());
        eno.setAssignment2(query.value("Assignment_2").toString());
        eno.setFinalGrade(query.value("final").toString());
        eno.setTotalMarks(query.value("total_marks").toInt());
        eno.setIsRafaaApplied(query.value("is_rafaa_applied").toInt() != 0);

        QDateTime enrolledAt = query.value("enrolled_at").toDateTime();
        if (enrolledAt.isValid()) eno.setEnrolledAt(enrolledAt);

        QDateTime createdAt = query.value("created_at").toDateTime();
        if (createdAt.isValid()) eno.setCreatedAt(createdAt);

        list.append(eno);
    }
    return list;
}
