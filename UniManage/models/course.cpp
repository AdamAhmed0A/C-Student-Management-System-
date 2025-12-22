#include "course.h"

Course::Course()
    : m_id(0), m_yearLevel(1), m_creditHours(3), m_semesterId(0), m_departmentId(0), m_maxGrade(100), m_courseType("Theoretical")
{
}

Course::Course(int id, const QString &name, const QString &description, int yearLevel, int creditHours, int semesterId)
{
    m_id = id;
    m_name = name;
    m_description = description;
    m_yearLevel = yearLevel;
    m_creditHours = creditHours;
    m_semesterId = semesterId;
    m_departmentId = 0;
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime();
}

int Course::id() const
{
    return m_id;
}

void Course::setId(int id)
{
    m_id = id;
}

QString Course::name() const
{
    return m_name;
}

void Course::setName(const QString &name)
{
    m_name = name;
}

QString Course::description() const
{
    return m_description;
}

void Course::setDescription(const QString &description)
{
    m_description = description;
}

int Course::yearLevel() const
{
    return m_yearLevel;
}

void Course::setYearLevel(int yearLevel)
{
    m_yearLevel = yearLevel;
}

int Course::creditHours() const
{
    return m_creditHours;
}

void Course::setCreditHours(int creditHours)
{
    m_creditHours = creditHours;
}

int Course::semesterId() const
{
    return m_semesterId;
}

void Course::setSemesterId(int semesterId)
{
    m_semesterId = semesterId;
}

QDateTime Course::createdAt() const
{
    return m_createdAt;
}

void Course::setCreatedAt(const QDateTime &createdAt)
{
    m_createdAt = createdAt;
}

QDateTime Course::updatedAt() const
{
    return m_updatedAt;
}

void Course::setUpdatedAt(const QDateTime &updatedAt)
{
    m_updatedAt = updatedAt;
}

QString Course::semesterName() const
{
    return m_semesterName;
}

void Course::setSemesterName(const QString &name)
{
    m_semesterName = name;
}

int Course::maxGrade() const { return m_maxGrade; }
void Course::setMaxGrade(int maxGrade) { m_maxGrade = maxGrade; }

QString Course::courseType() const { return m_courseType; }
void Course::setCourseType(const QString& courseType) { m_courseType = courseType; }

QString Course::assignedProfessor() const { return m_assignedProfessor; }
void Course::setAssignedProfessor(const QString& name) { m_assignedProfessor = name; }

QString Course::semesterYear() const { return m_semesterYear; }
void Course::setSemesterYear(const QString& year) { m_semesterYear = year; }

int Course::semesterNumber() const { return m_semesterNumber; }
void Course::setSemesterNumber(int number) { m_semesterNumber = number; }

int Course::departmentId() const { return m_departmentId; }
void Course::setDepartmentId(int departmentId) { m_departmentId = departmentId; }

QString Course::departmentName() const { return m_departmentName; }
void Course::setDepartmentName(const QString &name) { m_departmentName = name; }
