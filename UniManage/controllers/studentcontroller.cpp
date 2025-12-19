#include "studentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

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
    query.addBindValue(student.year());
    query.addBindValue(student.department());
    
    // Handle nullable section_id
    if (student.sectionId() <= 0) {
        query.addBindValue(QVariant()); // Bind NULL
    } else {
        query.addBindValue(student.sectionId());
    }
    
    query.addBindValue(student.seatNumber());
    query.addBindValue(student.status());
	if (!query.exec()) {
        QString errText = query.lastError().text();
        qDebug() << "Error adding student data:" << errText;
        if (error) *error = errText;
        return false;
	}
	return true;
}

//function to update student data
bool StudentController::updateStudent(const StudentData& student)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_STUDENT_DATA);
    query.addBindValue(student.idNumber());
    query.addBindValue(student.dob());
    query.addBindValue(student.year());
    query.addBindValue(student.department());
    query.addBindValue(student.sectionId());
    query.addBindValue(student.seatNumber());
    query.addBindValue(student.status());
    query.addBindValue(student.id());
    if (!query.exec()) {
        qDebug() << "Error updating student data:" << query.lastError().text();
        return false;
	}
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
    return true;
}

//function to get all students data
QList<StudentData> StudentController::getAllStudents()
{
	QList<StudentData> students;
	QSqlQuery query(DBConnection::instance().database());
    if (!query.exec(Queries::SELECT_ALL_STUDENTS_DATA)) {
        qDebug() << "Error retrieving students data:" << query.lastError().text();
        return students;
    }
    
    while (query.next()) {
        StudentData student;
        student.setId(query.value("id").toInt());
        student.setUserId(query.value("user_id").toInt());
        student.setStudentNumber(query.value("student_number").toString());
        student.setIdNumber(query.value("id_number").toString());
        student.setDob(query.value("dob").toDateTime());
        student.setYear(query.value("year").toInt());
        student.setDepartment(query.value("department").toString());
        student.setSectionId(query.value("section_id").toInt());
        student.setSeatNumber(query.value("seat_number").toString());
        student.setStatus(query.value("status").toString());
        student.setCreatedAt(query.value("created_at").toDateTime());
        student.setUpdatedAt(query.value("updated_at").toDateTime());
        // Additional user info
        student.setFullName(query.value("full_name").toString());
        student.setUsername(query.value("username").toString());
        
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
        student.setYear(query.value("year").toInt());
        student.setDepartment(query.value("department").toString());
        student.setSectionId(query.value("section_id").toInt());
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
        student.setYear(query.value("year").toInt());
        student.setDepartment(query.value("department").toString());
        student.setSectionId(query.value("section_id").toInt());
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
        student.setYear(query.value("year").toInt());
        student.setDepartment(query.value("department").toString());
        student.setSectionId(query.value("section_id").toInt());
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
