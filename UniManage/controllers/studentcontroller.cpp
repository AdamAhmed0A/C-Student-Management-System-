#include "studentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include "../database/persistence.h"

StudentController::StudentController() {}

//function to add student data
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
    Persistence::logChange("Student", "Enroll", query.lastInsertId().toInt(), student.studentNumber());
	return true;
}

//function to update student data
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

//function to delete student data
bool StudentController::deleteStudent(int id)
{
   QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_STUDENT_DATA);
	query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error deleting student data:" << query.lastError().text();
        return false;
	}
    Persistence::logChange("Student", "Delete", id, "Removed Student");
    return true;
}

//function to get all students data
QList<StudentData> StudentController::getAllStudents()
{
	QList<StudentData> students;
	QSqlQuery query(DBConnection::instance().database());
    
    qDebug() << "=== EXECUTING STUDENT QUERY ===";
    qDebug() << "Query:" << Queries::SELECT_ALL_STUDENTS_DATA;
    
    if (!query.exec(Queries::SELECT_ALL_STUDENTS_DATA)) {
        qDebug() << "Error retrieving students data:" << query.lastError().text();
        // Fallback: Try a simpler query if the complexity is failing due to joins or missing columns
        // This ensures the admin can still see the user accounts even if profile data has issues
        QString fallback = "SELECT u.id AS user_id, 0 AS id, u.full_name, u.username, u.role, u.username AS student_number, "
                           "'' AS id_number, NULL AS dob, '' AS department, 0 AS department_id, 0 AS academic_level_id, "
                           "0 AS section_id, 0 AS college_id, 0.0 AS tuition_fees, '' AS seat_number, 'Incomplete Profile' AS status, "
                           "u.created_at, u.updated_at, '' AS dept_name, '' AS level_name, '' AS college_name "
                           "FROM users u WHERE u.role = 'student'";
        if (!query.exec(fallback)) {
            return students;
        }
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

//function to get student data by id
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
	}
	return student;
}

//function to get student data by user id
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
	}
	return student;
}

//function to get student data by id number
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
    }
    return student;
}
