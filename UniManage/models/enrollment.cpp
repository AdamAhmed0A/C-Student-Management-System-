#include "enrollment.h"

Enrollment::Enrollment()
    : m_id(0), m_studentId(0), m_totalMarks(0), m_isRafaaApplied(false)
{
}

Enrollment::Enrollment(int id, int studentId, const QString& status, const QString& assessmentsGrades,
    const QString& assignment1, const QString& assignment2, const QString& finalGrade,
    int totalMarks, bool isRafaaApplied, const QDateTime& enrolledAt, const QDateTime& createdAt)
{
    m_id = id;
    m_studentId = studentId;
    m_status = status;
    m_assessmentsGrades = assessmentsGrades;
    m_assignment1 = assignment1;
    m_assignment2 = assignment2;
    m_final = finalGrade;
    m_totalMarks = totalMarks;
    m_isRafaaApplied = isRafaaApplied;
    m_enrolledAt = enrolledAt;
    m_createdAt = createdAt;
}

int Enrollment::id() const
{
    return m_id;
}

void Enrollment::setId(int id)
{
    m_id = id;
}

int Enrollment::studentId() const
{
    return m_studentId;
}

void Enrollment::setStudentId(int studentId)
{
    m_studentId = studentId;
}

QString Enrollment::status() const
{
    return m_status;
}

void Enrollment::setStatus(const QString& status)
{
    m_status = status;
}

QString Enrollment::assessmentsGrades() const
{
    return m_assessmentsGrades;
}

void Enrollment::setAssessmentsGrades(const QString& grades)
{
    m_assessmentsGrades = grades;
}

QString Enrollment::assignment1() const
{
    return m_assignment1;
}

void Enrollment::setAssignment1(const QString& val)
{
    m_assignment1 = val;
}

QString Enrollment::assignment2() const
{
    return m_assignment2;
}

void Enrollment::setAssignment2(const QString& val)
{
    m_assignment2 = val;
}

QString Enrollment::finalGrade() const
{
    return m_final;
}

void Enrollment::setFinalGrade(const QString& val)
{
    m_final = val;
}

int Enrollment::totalMarks() const
{
    return m_totalMarks;
}

void Enrollment::setTotalMarks(int totalMarks)
{
    m_totalMarks = totalMarks;
}

bool Enrollment::isRafaaApplied() const
{
    return m_isRafaaApplied;
}

void Enrollment::setIsRafaaApplied(bool applied)
{
    m_isRafaaApplied = applied;
}

QDateTime Enrollment::enrolledAt() const
{
    return m_enrolledAt;
}

void Enrollment::setEnrolledAt(const QDateTime& enrolledAt)
{
    m_enrolledAt = enrolledAt;
}

QDateTime Enrollment::createdAt() const
{
    return m_createdAt;
}

void Enrollment::setCreatedAt(const QDateTime& createdAt)
{
    m_createdAt = createdAt;
}
