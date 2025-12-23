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
    query.addBindValue(e.experienceGrade());
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
    query.prepare("UPDATE enrollments SET status = :status, "
                  "attendance_count = :att, absence_count = :abs, "
                  "assignment_1_grade = :a1, assignment_2_grade = :a2, "
                  "coursework_grade = :cw, final_exam_grade = :fi, "
                  "experience_grade = :exp, total_grade = :tot, "
                  "letter_grade = :lg WHERE id = :id");
    
    query.bindValue(":status", e.status().isEmpty() ? "active" : e.status());
    query.bindValue(":att", e.attendanceCount());
    query.bindValue(":abs", e.absenceCount());
    query.bindValue(":a1", e.assignment1Grade());
    query.bindValue(":a2", e.assignment2Grade());
    query.bindValue(":cw", e.courseworkGrade());
    query.bindValue(":fi", e.finalExamGrade());
    query.bindValue(":exp", e.experienceGrade());
    query.bindValue(":tot", e.totalGrade());
    query.bindValue(":lg", e.letterGrade().isEmpty() ? "N/A" : e.letterGrade());
    query.bindValue(":id", e.id());

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
            e.setExperienceGrade(query.value("experience_grade").toDouble());
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
            e.setExperienceGrade(query.value("experience_grade").toDouble());
            e.setTotalGrade(query.value("total_grade").toDouble());
            e.setLetterGrade(query.value("letter_grade").toString());
            e.setStudentName(query.value("full_name").toString());
            e.setStudentSection(query.value("section_name").toString());
            e.setStudentLevel(query.value("level_name").toString());
            e.setStudentCode(query.value("student_number").toString());
            e.setAcademicYear(query.value("academic_year").toString());
            list.append(e);
        }
    }
    return list;
}

Enrollment EnrollmentController::getEnrollmentById(int id)
{
    Enrollment e;
    QSqlQuery query(DBConnection::instance().database());
    // Explicitly select with table prefixes to avoid ambiguity during JOINs
    query.prepare("SELECT e.id, e.student_id, e.course_id, e.status, "
                  "e.attendance_count, e.absence_count, e.assignment_1_grade, "
                  "e.assignment_2_grade, e.coursework_grade, e.final_exam_grade, "
                  "e.experience_grade, e.total_grade, e.letter_grade, u.full_name "
                  "FROM enrollments e "
                  "JOIN students_data sd ON e.student_id = sd.id "
                  "JOIN users u ON sd.user_id = u.id "
                  "WHERE e.id = ?");
    query.addBindValue(id);
    
    if (query.exec() && query.next()) {
        e.setId(query.value(0).toInt());
        e.setStudentId(query.value(1).toInt());
        e.setCourseId(query.value(2).toInt());
        e.setStatus(query.value(3).toString());
        e.setAttendanceCount(query.value(4).toInt());
        e.setAbsenceCount(query.value(5).toInt());
        e.setAssignment1Grade(query.value(6).toDouble());
        e.setAssignment2Grade(query.value(7).toDouble());
        e.setCourseworkGrade(query.value(8).toDouble());
        e.setFinalExamGrade(query.value(9).toDouble());
        e.setExperienceGrade(query.value(10).toDouble());
        e.setTotalGrade(query.value(11).toDouble());
        e.setLetterGrade(query.value(12).toString());
        e.setStudentName(query.value(13).toString());
    }
    return e;
}

void EnrollmentController::calculateTotalAndGrade(Enrollment& e, const QString& courseType, int maxMarks)
{
    // Total is simply the sum of all components
    // The inputs (Assignment 1, 2, CW, Final) are expected to be raw marks out of their respective component max
    double total = e.assignment1Grade() + 
                   e.assignment2Grade() + 
                   e.courseworkGrade() + 
                   e.finalExamGrade() +
                   e.experienceGrade();

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

