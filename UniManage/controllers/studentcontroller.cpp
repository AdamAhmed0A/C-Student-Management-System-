#include "studentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include "../database/persistence.h"

/**
 * Constructor for the StudentController class
 */
StudentController::StudentController() {}

//function to add student data
/**
 * Adds a new student profile to the database
 * Also handles auto-enrollment in courses based on the academic level
 * Logs the creation event after successful insertion
 * @param student - The StudentData object containing details
 * @param error - Optional string pointer to store error messages
 * @return True if successful, otherwise false
 */
bool StudentController::addStudent(const StudentData& student, QString* error)
{
	QSqlQuery query(DBConnection::instance().database());
	query.prepare(Queries::INSERT_STUDENT_DATA);
    query.addBindValue(student.userId());
    query.addBindValue(student.studentNumber());
    query.addBindValue(student.idNumber());
    query.addBindValue(student.dob());
    query.addBindValue(student.department());
    
    // department_id
    if (student.departmentId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.departmentId());

    // academic_level_id
    if (student.academicLevelId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.academicLevelId());

    // Handle nullable section_id
    if (student.sectionId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.sectionId());

    // college_id
    if (student.collegeId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.collegeId());

    query.addBindValue(student.tuitionFees());
    query.addBindValue(student.seatNumber());
    query.addBindValue(student.status());
	if (!query.exec()) {
        QString errText = query.lastError().text();
        qDebug() << "Error adding student data:" << errText;
        if (error) *error = errText;
        return false;
	}

    int newStudentId = query.lastInsertId().toInt();
    
    // Auto-enroll in courses for this academic level
    if (student.academicLevelId() > 0) {
        enrollStudentInLevelCourses(newStudentId, student.academicLevelId());
    }

    Persistence::logChange("Student", "Enroll", newStudentId, student.studentNumber());
	return true;
}

//function to update student data
/**
 * Updates an existing student profile in the database
 * Logs the update event after successful execution
 * @param student - The StudentData object with updated details
 * @return True if successful, otherwise false
 */
bool StudentController::updateStudent(const StudentData& student)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_STUDENT_DATA);
    query.addBindValue(student.idNumber());
    query.addBindValue(student.dob());
    query.addBindValue(student.department());
    
    if (student.departmentId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.departmentId());

    if (student.academicLevelId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.academicLevelId());

    query.addBindValue(student.sectionId());
    if (student.collegeId() <= 0) query.addBindValue(QVariant());
    else query.addBindValue(student.collegeId());
    query.addBindValue(student.tuitionFees());
    query.addBindValue(student.seatNumber());
    query.addBindValue(student.status());
    query.addBindValue(student.id());
    if (!query.exec()) {
        qDebug() << "Error updating student data:" << query.lastError().text();
        return false;
	}
    Persistence::logChange("Student", "Edit", student.id(), student.studentNumber());
	return true;
}

//function to delete student data (Soft Delete)
/**
 * Soft deletes a student by moving them to a "deleted/draft" state
 * Does not remove data permanently, just hides it from active views
 * @param id - The ID of the student to soft delete
 * @return True if successful, otherwise false
 */
bool StudentController::deleteStudent(int id)
{
   QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SOFT_DELETE_STUDENT_DATA);
	query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error soft deleting student data:" << query.lastError().text();
        return false;
	}
    Persistence::logChange("Student", "Trash", id, "Moved to Draft");
    return true;
}

// Restore student from draft
/**
 * Restores a student from the "deleted/draft" state back to active
 * @param id - The ID of the student to restore
 * @return True if successful, otherwise false
 */
bool StudentController::restoreStudent(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::RESTORE_STUDENT_DATA);
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error restoring student data:" << query.lastError().text();
        return false;
    }
    Persistence::logChange("Student", "Restore", id, "Restored from Draft");
    return true;
}

// Permanent delete (incl. user account)
/**
 * Permanently deletes a student and all associated data
 * Cascades to payments, attendance, enrollments, and user account
 * @param id - The ID of the student to permanently delete
 * @return True if successful, otherwise false
 */
bool StudentController::hardDeleteStudent(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    
    // 1. Get user_id first
    int userId = -1;
    query.prepare("SELECT user_id FROM students_data WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        userId = query.value(0).toInt();
    }

    // 2. Cascade Delete Dependencies (Payments, Attendance, Enrollments)
    // Delete payments
    query.prepare("DELETE FROM payments WHERE student_id = ?");
    query.addBindValue(id);
    query.exec();
    
    // Delete attendance logs (via enrollments)
    query.prepare("DELETE FROM attendance_logs WHERE enrollment_id IN (SELECT id FROM enrollments WHERE student_id = ?)");
    query.addBindValue(id);
    query.exec();
    
    // Delete enrollments
    query.prepare("DELETE FROM enrollments WHERE student_id = ?");
    query.addBindValue(id);
    query.exec();

    // 3. Delete student profile
    query.prepare(Queries::DELETE_STUDENT_DATA);
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error permanently deleting student data:" << query.lastError().text();
        return false;
    }

    // 4. Delete user account if found
    if (userId > 0) {
        query.prepare(Queries::DELETE_USER);
        query.addBindValue(userId);
        query.exec();
    }

    Persistence::logChange("Student", "HardDelete", id, "Permanently Removed Profile and User");
    return true;
}

//function to get all students data
/**
 * Retrieves all active students from the database
 * Joins with multiple tables to provide comprehensive details
 * @return A list of StudentData objects
 */
QList<StudentData> StudentController::getAllStudents()
{
	QList<StudentData> students;
	QSqlQuery query(DBConnection::instance().database());
    
    qDebug() << "=== EXECUTING STUDENT QUERY ===";
    qDebug() << "Query:" << Queries::SELECT_ALL_STUDENTS_DATA;
    
    if (!query.exec(Queries::SELECT_ALL_STUDENTS_DATA)) {
        qDebug() << "Error retrieving students data:" << query.lastError().text();
        return students;
    }
    
    qDebug() << "Query executed successfully!";
    int count = 0;
    
    while (query.next()) {
        count++;
        StudentData student;
        student.setId(query.value("id").toInt());
        student.setUserId(query.value("user_id").toInt());
        student.setStudentNumber(query.value("student_number").toString());
        student.setIdNumber(query.value("id_number").toString());
        student.setDob(query.value("dob").toDateTime());
        student.setDepartment(query.value("department").toString());
        student.setDepartmentId(query.value("department_id").toInt());
        student.setAcademicLevelId(query.value("academic_level_id").toInt());
        student.setSectionId(query.value("section_id").toInt());
        student.setCollegeId(query.value("college_id").toInt());
        student.setTuitionFees(query.value("tuition_fees").toDouble());
        student.setSeatNumber(query.value("seat_number").toString());
        student.setStatus(query.value("status").toString());
        student.setCreatedAt(query.value("created_at").toDateTime());
        student.setUpdatedAt(query.value("updated_at").toDateTime());
        
        // Additional info from joins
        student.setFullName(query.value("full_name").toString());
        student.setUsername(query.value("username").toString());
        student.setRole(query.value("role").toString());
        
        QString dName = query.value("dept_name").toString();
        if(!dName.isEmpty()) student.setDepartment(dName);
        
        student.setLevelName(query.value("level_name").toString());
        student.setCollegeName(query.value("college_name").toString());
        student.setSectionName(query.value("section_name").toString());
        
        students.append(student);
    }
    
	return students;
}

// Get students in draft/trash
/**
 * Retrieves deleted students (students in the "Trash" or "Draft" state)
 * @return A list of StudentData objects
 */
QList<StudentData> StudentController::getDeletedStudents()
{
    QList<StudentData> students;
    QSqlQuery query(DBConnection::instance().database());
    
    if (!query.exec(Queries::SELECT_DELETED_STUDENTS_DATA)) {
        qDebug() << "Error retrieving deleted students data:" << query.lastError().text();
        return students;
    }
    
    while (query.next()) {
        StudentData student;
        student.setId(query.value("id").toInt());
        student.setUserId(query.value("user_id").toInt());
        student.setStudentNumber(query.value("student_number").toString());
        student.setIdNumber(query.value("id_number").toString());
        student.setDob(query.value("dob").toDateTime());
        student.setDepartment(query.value("department").toString());
        student.setDepartmentId(query.value("department_id").toInt());
        student.setAcademicLevelId(query.value("academic_level_id").toInt());
        student.setSectionId(query.value("section_id").toInt());
        student.setCollegeId(query.value("college_id").toInt());
        student.setTuitionFees(query.value("tuition_fees").toDouble());
        student.setSeatNumber(query.value("seat_number").toString());
        student.setStatus(query.value("status").toString());
        student.setCreatedAt(query.value("created_at").toDateTime());
        student.setUpdatedAt(query.value("updated_at").toDateTime());
        
        student.setFullName(query.value("full_name").toString());
        student.setUsername(query.value("username").toString());
        student.setRole(query.value("role").toString());
        
        QString dName = query.value("dept_name").toString();
        if(!dName.isEmpty()) student.setDepartment(dName);
        
        student.setLevelName(query.value("level_name").toString());
        student.setCollegeName(query.value("college_name").toString());
        student.setSectionName(query.value("section_name").toString());
        
        students.append(student);
    }
    return students;
}

//function to get student data by id
/**
 * Retrieves a student profile by their unique student data ID
 * @param id - The ID of the student data
 * @return The StudentData object if found, otherwise an empty object
 */
StudentData StudentController::getStudentById(int id)
{
	StudentData student;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_STUDENT_DATA_BY_ID);
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error retrieving student data by ID:" << query.lastError().text();
        return student;
    }
    
    if (query.next()) {
        student.setId(query.value("id").toInt());
        student.setUserId(query.value("user_id").toInt());
        student.setStudentNumber(query.value("student_number").toString());
        student.setIdNumber(query.value("id_number").toString());
        student.setDob(query.value("dob").toDateTime());
        student.setDepartment(query.value("department").toString());
        student.setDepartmentId(query.value("department_id").toInt());
        student.setAcademicLevelId(query.value("academic_level_id").toInt());
        student.setSectionId(query.value("section_id").toInt());
        student.setCollegeId(query.value("college_id").toInt());
        student.setTuitionFees(query.value("tuition_fees").toDouble());
        student.setSeatNumber(query.value("seat_number").toString());
        student.setStatus(query.value("status").toString());
        student.setCreatedAt(query.value("created_at").toDateTime());
        student.setUpdatedAt(query.value("updated_at").toDateTime());
        // Additional user info
        student.setFullName(query.value("full_name").toString());
        student.setUsername(query.value("username").toString());
        
        QString dName = query.value("dept_name").toString();
        if(!dName.isEmpty()) student.setDepartment(dName);
        student.setLevelName(query.value("level_name").toString());
        student.setCollegeName(query.value("college_name").toString());
        student.setSectionName(query.value("section_name").toString());
	}
	return student;
}

//function to get student data by user id
/**
 * Retrieves a student profile by their user ID
 * @param userId - The ID of the user
 * @return The StudentData object if found, otherwise an empty object
 */
StudentData StudentController::getStudentByUserId(int userId)
{
	StudentData student;
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_STUDENT_DATA_BY_USER_ID);
    query.addBindValue(userId);
    if (!query.exec()) {
        qDebug() << "Error retrieving student data by user ID:" << query.lastError().text();
        return student;
    }
    
    if (query.next()) {
        student.setId(query.value("id").toInt());
        student.setUserId(query.value("user_id").toInt());
        student.setStudentNumber(query.value("student_number").toString());
        student.setIdNumber(query.value("id_number").toString());
        student.setDob(query.value("dob").toDateTime());
        student.setDepartment(query.value("department").toString());
        student.setDepartmentId(query.value("department_id").toInt());
        student.setAcademicLevelId(query.value("academic_level_id").toInt());
        student.setSectionId(query.value("section_id").toInt());
        student.setCollegeId(query.value("college_id").toInt());
        student.setTuitionFees(query.value("tuition_fees").toDouble());
        student.setSeatNumber(query.value("seat_number").toString());
        student.setStatus(query.value("status").toString());
        student.setCreatedAt(query.value("created_at").toDateTime());
        student.setUpdatedAt(query.value("updated_at").toDateTime());
        // Additional user info
        student.setFullName(query.value("full_name").toString());
        student.setUsername(query.value("username").toString());
        
        QString dName = query.value("dept_name").toString();
        if(!dName.isEmpty()) student.setDepartment(dName);
        student.setLevelName(query.value("level_name").toString());
        student.setCollegeName(query.value("college_name").toString());
        student.setSectionName(query.value("section_name").toString());
	}
	return student;
}

//function to get student data by id number
/**
 * Retrieves a student profile by their unique student number
 * @param IdNumber - The student number (as integer)
 * @return The StudentData object if found, otherwise an empty object
 */
StudentData StudentController::getStudentByIdNumber(int IdNumber)
{
    StudentData student;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_STUDENT_DATA_BY_STUDENT_NUMBER);
    query.addBindValue(IdNumber);
    if (!query.exec()) {
        qDebug() << "Error retrieving student data by ID number:" << query.lastError().text();
        return student;
    }
    
    if (query.next()) {
        student.setId(query.value("id").toInt());
        student.setUserId(query.value("user_id").toInt());
        student.setStudentNumber(query.value("student_number").toString());
        student.setIdNumber(query.value("id_number").toString());
        student.setDob(query.value("dob").toDateTime());
        student.setDepartment(query.value("department").toString());
        student.setDepartmentId(query.value("department_id").toInt());
        student.setAcademicLevelId(query.value("academic_level_id").toInt());
        student.setSectionId(query.value("section_id").toInt());
        student.setCollegeId(query.value("college_id").toInt());
        student.setTuitionFees(query.value("tuition_fees").toDouble());
        student.setSeatNumber(query.value("seat_number").toString());
        student.setStatus(query.value("status").toString());
        student.setCreatedAt(query.value("created_at").toDateTime());
        student.setUpdatedAt(query.value("updated_at").toDateTime());
        // Additional user info
        student.setFullName(query.value("full_name").toString());
        student.setUsername(query.value("username").toString());

        QString dName = query.value("dept_name").toString();
        if(!dName.isEmpty()) student.setDepartment(dName);
        student.setLevelName(query.value("level_name").toString());
        student.setCollegeName(query.value("college_name").toString());
        student.setSectionName(query.value("section_name").toString());
    }
    return student;
}

/**
 * Automatically enrolls a student in all courses associated with their academic level
 * @param studentId - The ID of the student
 * @param levelId - The ID of the academic level
 * @return True if successful, otherwise false
 */
bool StudentController::enrollStudentInLevelCourses(int studentId, int levelId) {
    if (studentId <= 0 || levelId <= 0) return false;
    
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_ENROLLMENT_BATCH_BY_LEVEL);
    query.addBindValue(studentId);
    query.addBindValue(levelId);
    
    if (!query.exec()) {
        QString err = query.lastError().text();
        // Ignore "Duplicate" errors if the student is already enrolled in some
        if (!err.contains("Duplicate", Qt::CaseInsensitive)) {
            qDebug() << "Auto-enrollment failed:" << err;
            return false;
        }
    }
    return true;
}
