#include "course.h"

/**
 * Default Constructor
 * Initializes a new instance with default values
 * Defaults: m_yearLevel=1, m_creditHours=3, m_maxGrade=100, m_courseType="Theoretical"
 */
Course::Course()
    : m_id(0), m_yearLevel(1), m_creditHours(3), m_semesterId(0), m_departmentId(0), m_maxGrade(100), m_courseType("Theoretical")
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the course
 * @param name - The name of the course
 * @param description - A brief description of the course content
 * @param yearLevel - The academic year level (e.g., 1, 2, 3...)
 * @param creditHours - The number of credit hours for the course
 * @param semesterId - The ID of the semester this course belongs to
 */
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

/**
 * @return The unique ID of the course
 */
int Course::id() const
{
    return m_id;
}

/**
 * Sets the unique ID of the course
 * @param id - The new ID
 */
void Course::setId(int id)
{
    m_id = id;
}

/**
 * @return The name of the course
 */
QString Course::name() const
{
    return m_name;
}

/**
 * Sets the name of the course
 * @param name - The new name
 */
void Course::setName(const QString &name)
{
    m_name = name;
}

/**
 * @return The description of the course
 */
QString Course::description() const
{
    return m_description;
}

/**
 * Sets the description of the course
 * @param description - The new description
 */
void Course::setDescription(const QString &description)
{
    m_description = description;
}

/**
 * @return The academic year level of the course
 */
int Course::yearLevel() const
{
    return m_yearLevel;
}

/**
 * Sets the academic year level of the course
 * @param yearLevel - The new year level
 */
void Course::setYearLevel(int yearLevel)
{
    m_yearLevel = yearLevel;
}

/**
 * @return The credit hours of the course
 */
int Course::creditHours() const
{
    return m_creditHours;
}

/**
 * Sets the credit hours of the course
 * @param creditHours - The new credit hours
 */
void Course::setCreditHours(int creditHours)
{
    m_creditHours = creditHours;
}

/**
 * @return The ID of the semester the course is assigned to
 */
int Course::semesterId() const
{
    return m_semesterId;
}

/**
 * Sets the semester ID for the course
 * @param semesterId - The new semester ID
 */
void Course::setSemesterId(int semesterId)
{
    m_semesterId = semesterId;
}

/**
 * @return The date and time when the course was created
 */
QDateTime Course::createdAt() const
{
    return m_createdAt;
}

/**
 * Sets the creation timestamp
 * @param createdAt - The creation date and time
 */
void Course::setCreatedAt(const QDateTime &createdAt)
{
    m_createdAt = createdAt;
}

/**
 * @return The date and time when the course was last updated
 */
QDateTime Course::updatedAt() const
{
    return m_updatedAt;
}

/**
 * Sets the update timestamp
 * @param updatedAt - The update date and time
 */
void Course::setUpdatedAt(const QDateTime &updatedAt)
{
    m_updatedAt = updatedAt;
}

/**
 * @return The name of the semester (e.g., "Fall 2024") - derived or joined
 */
QString Course::semesterName() const
{
    return m_semesterName;
}

/**
 * Sets the semester name
 * @param name - The semester name
 */
void Course::setSemesterName(const QString &name)
{
    m_semesterName = name;
}

/**
 * @return The maximum grade possible for the course
 */
int Course::maxGrade() const { return m_maxGrade; }

/**
 * Sets the maximum grade for the course
 * @param maxGrade - The new maximum grade
 */
void Course::setMaxGrade(int maxGrade) { m_maxGrade = maxGrade; }

/**
 * @return The type of course (e.g., "Theoretical", "Practical")
 */
QString Course::courseType() const { return m_courseType; }

/**
 * Sets the course type
 * @param courseType - The new course type
 */
void Course::setCourseType(const QString& courseType) { m_courseType = courseType; }

/**
 * @return The name of the professor assigned to this course
 */
QString Course::assignedProfessor() const { return m_assignedProfessor; }

/**
 * Sets the assigned professor's name
 * @param name - The professor's name
 */
void Course::setAssignedProfessor(const QString& name) { m_assignedProfessor = name; }

/**
 * @return The academic year of the semester (e.g., "2023-2024")
 */
QString Course::semesterYear() const { return m_semesterYear; }

/**
 * Sets the academic year string
 * @param year - The new year string
 */
void Course::setSemesterYear(const QString& year) { m_semesterYear = year; }

/**
 * @return The semester number (e.g., 1 or 2)
 */
int Course::semesterNumber() const { return m_semesterNumber; }

/**
 * Sets the semester number
 * @param number - The new semester number
 */
void Course::setSemesterNumber(int number) { m_semesterNumber = number; }

/**
 * @return The ID of the department offering the course
 */
int Course::departmentId() const { return m_departmentId; }

/**
 * Sets the department ID
 * @param departmentId - The new department ID
 */
void Course::setDepartmentId(int departmentId) { m_departmentId = departmentId; }

/**
 * @return The name of the department offering the course
 */
QString Course::departmentName() const { return m_departmentName; }

/**
 * Sets the department name
 * @param name - The new department name
 */
void Course::setDepartmentName(const QString &name) { m_departmentName = name; }
