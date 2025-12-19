#include "enrollmentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

EnrollmentController::EnrollmentController() {}

bool EnrollmentController::addEnrollment(const Enrollment& e)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_ENROLLMENT);
    query.addBindValue(e.studentId());
    query.addBindValue(e.courseId());
    query.addBindValue(e.status());
    query.addBindValue(e.attendanceCount());
    query.addBindValue(e.absenceCount());
    query.addBindValue(e.assignment1Grade());
    query.addBindValue(e.assignment2Grade());
    query.addBindValue(e.courseworkGrade());
    query.addBindValue(e.finalExamGrade());
    query.addBindValue(e.totalGrade());
    query.addBindValue(e.letterGrade());

    if (!query.exec()) {
        qDebug() << "addEnrollment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool EnrollmentController::updateEnrollment(const Enrollment& e)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_ENROLLMENT);
    query.addBindValue(e.status());
    query.addBindValue(e.attendanceCount());
    query.addBindValue(e.absenceCount());
    query.addBindValue(e.assignment1Grade());
    query.addBindValue(e.assignment2Grade());
    query.addBindValue(e.courseworkGrade());
    query.addBindValue(e.finalExamGrade());
    query.addBindValue(e.totalGrade());
    query.addBindValue(e.letterGrade());
    query.addBindValue(e.id());

    if (!query.exec()) {
        qDebug() << "updateEnrollment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool EnrollmentController::deleteEnrollment(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_ENROLLMENT);
    query.addBindValue(id);
    return query.exec();
}

QList<Enrollment> EnrollmentController::getEnrollmentsByStudent(int studentId)
{
    QList<Enrollment> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_ENROLLMENTS_BY_STUDENT);
    query.addBindValue(studentId);

    if (query.exec()) {
        while (query.next()) {
            Enrollment e;
            e.setId(query.value("id").toInt());
            e.setStudentId(query.value("student_id").toInt());
            e.setCourseId(query.value("course_id").toInt());
            e.setStatus(query.value("status").toString());
            e.setAttendanceCount(query.value("attendance_count").toInt());
            e.setAbsenceCount(query.value("absence_count").toInt());
            e.setAssignment1Grade(query.value("assignment_1_grade").toDouble());
            e.setAssignment2Grade(query.value("assignment_2_grade").toDouble());
            e.setCourseworkGrade(query.value("coursework_grade").toDouble());
            e.setFinalExamGrade(query.value("final_exam_grade").toDouble());
            e.setTotalGrade(query.value("total_grade").toDouble());
            e.setLetterGrade(query.value("letter_grade").toString());
            e.setEnrolledAt(query.value("enrolled_at").toDateTime());
            e.setCourseName(query.value("course_name").toString());
            list.append(e);
        }
    }
    return list;
}

QList<Enrollment> EnrollmentController::getEnrollmentsByCourse(int courseId)
{
    QList<Enrollment> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_ENROLLMENTS_BY_COURSE);
    query.addBindValue(courseId);

    if (query.exec()) {
        while (query.next()) {
            Enrollment e;
            e.setId(query.value("id").toInt());
            e.setStudentId(query.value("student_id").toInt());
            e.setCourseId(query.value("course_id").toInt());
            e.setAttendanceCount(query.value("attendance_count").toInt());
            e.setAbsenceCount(query.value("absence_count").toInt());
            e.setAssignment1Grade(query.value("assignment_1_grade").toDouble());
            e.setAssignment2Grade(query.value("assignment_2_grade").toDouble());
            e.setCourseworkGrade(query.value("coursework_grade").toDouble());
            e.setFinalExamGrade(query.value("final_exam_grade").toDouble());
            e.setTotalGrade(query.value("total_grade").toDouble());
            e.setLetterGrade(query.value("letter_grade").toString());
            e.setStudentName(query.value("full_name").toString());
            list.append(e);
        }
    }
    return list;
}

void EnrollmentController::calculateTotalAndGrade(Enrollment& e, const QString& courseType, int maxMarks)
{
    double total = 0;
    if (courseType == "Practical") {
        // Practical courses: Ass1 (20%), Ass2 (30%), CW (20%), Final (30%)
        total = (e.assignment1Grade() * 0.20) + 
                (e.assignment2Grade() * 0.30) + 
                (e.courseworkGrade() * 0.20) + 
                (e.finalExamGrade() * 0.30);
    } else {
        // Theoretical courses: Ass1 (20%), Ass2 (20%), CW (60%), Final (0%)
        total = (e.assignment1Grade() * 0.20) + 
                (e.assignment2Grade() * 0.20) + 
                (e.courseworkGrade() * 0.60);
    }

    e.setTotalGrade(total);

    // Calculate Percentage
    double percentage = (total / maxMarks) * 100.0;
    QString grade;
    if (percentage >= 85) grade = "Excellent";
    else if (percentage >= 75) grade = "Very Good";
    else if (percentage >= 65) grade = "Good";
    else if (percentage >= 60) grade = "Pass";
    else grade = "Fail";

    e.setLetterGrade(grade);
}
