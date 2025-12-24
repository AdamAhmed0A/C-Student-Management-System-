#include "semester.h"

/**
 * Default Constructor
 * Initializes a new semester instance with default values.
 */
Semester::Semester()
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the semester
 * @param year - The academic year associated with the semester
 * @param semester - The semester number (e.g., 1 for Fall, 2 for Spring)
 */
Semester::Semester(int id, const QDateTime& year, int semester)
{
    m_id = id;
    m_year = year;
    m_semester = semester;
}

/**
 * @return The unique ID of the semester
 */
int Semester::id() const
{
    return m_id;
}

/**
 * Sets the unique ID of the semester
 * @param id - The new ID
 */
void Semester::setId(int id)
{
    m_id = id;
}

/**
 * @return The academic year of the semester
 */
QDateTime Semester::year() const
{
    return m_year;
}

/**
 * Sets the academic year of the semester
 * @param year - The new academic year
 */
void Semester::setYear(const QDateTime& year)
{
    m_year = year;
}

/**
 * @return The semester number (e.g., 1 or 2)
 */
int Semester::semester() const
{
    return m_semester;
}

/**
 * Sets the semester number
 * @param semester - The new semester number
 */
void Semester::setSemester(int semester)
{
    m_semester = semester;
}

/**
 * @return The timestamp when the semester record was created
 */
QDateTime Semester::createdAt() const
{
    return m_createdAt;
}

/**
 * Sets the creation timestamp
 * @param createdAt - The new creation timestamp
 */
void Semester::setCreatedAt(const QDateTime& createdAt)
{
    m_createdAt = createdAt;
}

/**
 * @return The timestamp when the semester record was last updated
 */
QDateTime Semester::updatedAt() const
{
    return m_updatedAt;
}

/**
 * Sets the update timestamp
 * @param updatedAt - The new update timestamp
 */
void Semester::setUpdatedAt(const QDateTime& updatedAt)
{
    m_updatedAt = updatedAt;
}
