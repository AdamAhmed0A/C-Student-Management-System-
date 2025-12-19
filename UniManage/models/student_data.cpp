#include "student_data.h"

StudentData::StudentData()
    : m_id(0), m_userId(0), m_sectionId(0), m_departmentId(0), m_academicLevelId(0)
{
}

StudentData::StudentData(int id, int userId, const QString &studentNumber, const QString &idNumber, const QString &phone, const QDateTime &dob, const QString &department, int sectionId, const QString &seatNumber, const QString &status)
    : m_id(id), m_userId(userId), m_studentNumber(studentNumber), m_idNumber(idNumber), m_phone(phone), m_dob(dob), m_department(department), m_sectionId(sectionId), m_seatNumber(seatNumber), m_status(status), m_departmentId(0), m_academicLevelId(0)
{
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime();
}

int StudentData::id() const { return m_id; }
void StudentData::setId(int id) { m_id = id; }

int StudentData::userId() const { return m_userId; }
void StudentData::setUserId(int userId) { m_userId = userId; }

QString StudentData::studentNumber() const { return m_studentNumber; }
void StudentData::setStudentNumber(const QString &studentNumber) { m_studentNumber = studentNumber; }

QString StudentData::idNumber() const { return m_idNumber; }
void StudentData::setIdNumber(const QString &idNumber) { m_idNumber = idNumber; }

QString StudentData::phone() const { return m_phone; }
void StudentData::setPhone(const QString &phone) { m_phone = phone; }

QDateTime StudentData::dob() const { return m_dob; }
void StudentData::setDob(const QDateTime &dob) { m_dob = dob; }

QString StudentData::department() const { return m_department; }
void StudentData::setDepartment(const QString &department) { m_department = department; }

int StudentData::departmentId() const { return m_departmentId; }
void StudentData::setDepartmentId(int id) { m_departmentId = id; }

int StudentData::academicLevelId() const { return m_academicLevelId; }
void StudentData::setAcademicLevelId(int id) { m_academicLevelId = id; }

int StudentData::sectionId() const { return m_sectionId; }
void StudentData::setSectionId(int sectionId) { m_sectionId = sectionId; }

QString StudentData::seatNumber() const { return m_seatNumber; }
void StudentData::setSeatNumber(const QString &seatNumber) { m_seatNumber = seatNumber; }

QString StudentData::status() const { return m_status; }
void StudentData::setStatus(const QString &status) { m_status = status; }

QDateTime StudentData::createdAt() const { return m_createdAt; }
void StudentData::setCreatedAt(const QDateTime &createdAt) { m_createdAt = createdAt; }

QDateTime StudentData::updatedAt() const { return m_updatedAt; }
void StudentData::setUpdatedAt(const QDateTime &updatedAt) { m_updatedAt = updatedAt; }

QString StudentData::fullName() const { return m_fullName; }
void StudentData::setFullName(const QString &fullName) { m_fullName = fullName; }

QString StudentData::username() const { return m_username; }
void StudentData::setUsername(const QString &username) { m_username = username; }
