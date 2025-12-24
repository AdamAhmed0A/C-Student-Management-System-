#include "enrollmentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

/**
 * Constructor for the EnrollmentController class
 */
EnrollmentController::EnrollmentController() {}

/**
 * Adds a new student enrollment to a course
 * @param e - The Enrollment object containing details
 * @return True if successful, otherwise false
 */
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

/**
 * Updates an existing enrollment (grades, attendance, status)
 * @param e - The Enrollment object with updated details
 * @return True if successful, otherwise false
 */
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

/**
 * Deletes an enrollment from the database
 * @param id - The ID of the enrollment to delete
 * @return True if successful, otherwise false
 */
bool EnrollmentController::deleteEnrollment(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_ENROLLMENT);
    query.addBindValue(id);
    return query.exec();
}

/**
 * Retrieves all enrollments for a specific student
 * Includes details about the course and its type/grading scale
 * @param studentId - The ID of the student
 * @return A list of Enrollment objects
 */
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

/**
 * Retrieves all enrollments for a specific course
 * Includes detailed student info (name, section, level)
 * @param courseId - The ID of the course
 * @return A list of Enrollment objects
 */
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

/**
 * Retrieves a single enrollment by its ID
 * @param id - The ID of the enrollment
 * @return The Enrollment object if found, otherwise an empty object
 */
Enrollment EnrollmentController::getEnrollmentById(int id)
{
    Enrollment e;
    QSqlQuery query(DBConnection::instance().database());
    // Use a simpler query first to ensure we get the enrollment data
    query.prepare("SELECT * FROM enrollments WHERE id = ?");
    query.addBindValue(id);
    
    if (query.exec() && query.next()) {
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
        
        // Fetch student name separately for reliability
        QSqlQuery nameQuery(DBConnection::instance().database());
        nameQuery.prepare("SELECT u.full_name FROM students_data sd "
                         "JOIN users u ON sd.user_id = u.id "
                         "WHERE sd.id = ?");
        nameQuery.addBindValue(e.studentId());
        if (nameQuery.exec() && nameQuery.next()) {
            e.setStudentName(nameQuery.value(0).toString());
        }
    } else {
        qDebug() << "getEnrollmentById failed for ID:" << id << "Error:" << query.lastError().text();
    }
    return e;
}

/**
 * Calculates total grades and determines letter grade based on course max marks
 * Updates the enrollment object in place
 * @param e - Reference to the Enrollment object to be updated
 * @param courseType - Type of course (affects grading scale if needed in future)
 * @param maxMarks - Maximum marks for the course (e.g., 100 or 150)
 */
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

/**
 * records an attendance log for a specific date
 * Updates existing log if found, otherwise creates a new one
 * Recalculates total attendance counts and updates the main enrollment record
 * @param log - The AttendanceLog object containing the record
 * @return True if successful, otherwise false
 */
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

/**
 * Retrieves attendance logs for a specific course on a given date
 * @param courseId - The ID of the course
 * @param date - The date to fetch logs for
 * @return A list of AttendanceLog objects
 */
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

