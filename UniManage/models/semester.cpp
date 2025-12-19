#include "semester.h"

Semester::Semester()
{
}

Semester::Semester(int id, const QDateTime& year, int semester)
{
    m_id = id;
    m_year = year;
    m_semester = semester;
}

int Semester::id() const
{
    return m_id;
}

void Semester::setId(int id)
{
    m_id = id;
}

QDateTime Semester::year() const
{
    return m_year;
}

void Semester::setYear(const QDateTime& year)
{
    m_year = year;
}

int Semester::semester() const
{
    return m_semester;
}

void Semester::setSemester(int semester)
{
    m_semester = semester;
}

QDateTime Semester::createdAt() const
{
    return m_createdAt;
}

void Semester::setCreatedAt(const QDateTime& createdAt)
{
    m_createdAt = createdAt;
}

QDateTime Semester::updatedAt() const
{
    return m_updatedAt;
}

void Semester::setUpdatedAt(const QDateTime& updatedAt)
{
    m_updatedAt = updatedAt;
}
