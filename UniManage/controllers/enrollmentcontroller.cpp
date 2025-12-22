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
            e.setCourseMaxGrade(query.value("max_grade").toInt());
            e.setCourseType(query.value("course_type").toString());
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
            e.setStudentSection(query.value("section_name").toString());
            list.append(e);
        }
    }
    return list;
}

void EnrollmentController::calculateTotalAndGrade(Enrollment& e, const QString& courseType, int maxMarks)
{
    // Total is simply the sum of all components
    // The inputs (Assignment 1, 2, CW, Final) are expected to be raw marks out of their respective component max
    double total = e.assignment1Grade() + 
                   e.assignment2Grade() + 
                   e.courseworkGrade() + 
                   e.finalExamGrade();

    e.setTotalGrade(total);

    // Calculate Percentage based on course Max Marks (100 or 150)
    double percentage = 0.0;
    if (maxMarks > 0) {
        percentage = (total / maxMarks) * 100.0;
    }

    QString grade;
    if (percentage >= 85) grade = "Excellent";
    else if (percentage >= 75) grade = "Very Good";
    else if (percentage >= 65) grade = "Good";
    else if (percentage >= 60) grade = "Pass";
    else grade = "Fail";

    e.setLetterGrade(grade);
}

bool EnrollmentController::addAttendanceLog(const AttendanceLog& log)
{
    QSqlDatabase& db = DBConnection::instance().database();
    
    // 1. Check if log exists for this date
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT id FROM attendance_logs WHERE enrollment_id = ? AND date = ?");
    checkQuery.addBindValue(log.enrollmentId());
    checkQuery.addBindValue(log.date());
    
    bool exists = false;
    int existingId = -1;
    if (checkQuery.exec() && checkQuery.next()) {
        exists = true;
        existingId = checkQuery.value(0).toInt();
    }
    
    // 2. Insert or Update
    QSqlQuery query(db);
    if (exists) {
        query.prepare("UPDATE attendance_logs SET status = ?, notes = ? WHERE id = ?");
        query.addBindValue(log.status());
        query.addBindValue(log.notes());
        query.addBindValue(existingId);
    } else {
        query.prepare(Queries::INSERT_ATTENDANCE_LOG);
        query.addBindValue(log.enrollmentId());
        query.addBindValue(log.date());
        query.addBindValue(log.status());
        query.addBindValue(log.notes());
    }
    
    if (!query.exec()) {
        qDebug() << "addAttendanceLog (insert/update) failed:" << query.lastError().text();
        return false;
    }
    
    // 3. Recalculate counts
    QSqlQuery countQuery(db);
    int presentCount = 0;
    int absentCount = 0;
    
    countQuery.prepare("SELECT COUNT(*) FROM attendance_logs WHERE enrollment_id = ? AND status = 'Present'");
    countQuery.addBindValue(log.enrollmentId());
    if (countQuery.exec() && countQuery.next()) presentCount = countQuery.value(0).toInt();
    
    countQuery.prepare("SELECT COUNT(*) FROM attendance_logs WHERE enrollment_id = ? AND status = 'Absent'");
    countQuery.addBindValue(log.enrollmentId());
    if (countQuery.exec() && countQuery.next()) absentCount = countQuery.value(0).toInt();
    
    // 4. Update Enrollment
    QSqlQuery updateEnrollment(db);
    updateEnrollment.prepare("UPDATE enrollments SET attendance_count = ?, absence_count = ? WHERE id = ?");
    updateEnrollment.addBindValue(presentCount);
    updateEnrollment.addBindValue(absentCount);
    updateEnrollment.addBindValue(log.enrollmentId());
    
    if (!updateEnrollment.exec()) {
        qDebug() << "addAttendanceLog (update enrollment counts) failed:" << updateEnrollment.lastError().text();
        return false; 
    }
    
    return true;
}

QList<AttendanceLog> EnrollmentController::getAttendanceLogsByCourse(int courseId, const QDate& date)
{
    QList<AttendanceLog> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_ATTENDANCE_LOGS_BY_COURSE_DATE);
    query.addBindValue(courseId);
    query.addBindValue(date);
    
    if (query.exec()) {
        while (query.next()) {
            AttendanceLog log;
            log.setId(query.value("id").toInt());
            log.setEnrollmentId(query.value("enrollment_id").toInt());
            log.setDate(query.value("date").toDate());
            log.setStatus(query.value("status").toString());
            log.setNotes(query.value("notes").toString());
            list.append(log);
        }
    }
    return list;
}

