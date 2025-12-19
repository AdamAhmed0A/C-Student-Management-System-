#include "payment.h"

Payment::Payment()
    : m_id(0), m_studentId(0), m_amount(0.0f)
{
}

Payment::Payment(int id, int studentId, float amount, const QDateTime &date, const QDateTime &year,
                 const QString &method, const QString &status, const QString &notes,
                 const QDateTime &createdAt, const QString &studentNumber)
{
    m_id = id;
    m_studentId = studentId;
    m_amount = amount;
    m_date = date;
    m_year = year;
    m_method = method;
    m_status = status;
    m_notes = notes;
    m_createdAt = createdAt;
    m_studentNumber = studentNumber;
}

int Payment::id() const
{
    return m_id;
}

void Payment::setId(int id)
{
    m_id = id;
}

int Payment::studentId() const
{
    return m_studentId;
}

void Payment::setStudentId(int studentId)
{
    m_studentId = studentId;
}

float Payment::amount() const
{
    return m_amount;
}

void Payment::setAmount(float amount)
{
    m_amount = amount;
}

QDateTime Payment::date() const
{
    return m_date;
}

void Payment::setDate(const QDateTime &date)
{
    m_date = date;
}

QDateTime Payment::year() const
{
    return m_year;
}

void Payment::setYear(const QDateTime &year)
{
    m_year = year;
}

QString Payment::method() const
{
    return m_method;
}

void Payment::setMethod(const QString &method)
{
    m_method = method;
}

QString Payment::status() const
{
    return m_status;
}

void Payment::setStatus(const QString &status)
{
    m_status = status;
}

QString Payment::notes() const
{
    return m_notes;
}

void Payment::setNotes(const QString &notes)
{
    m_notes = notes;
}

QDateTime Payment::createdAt() const
{
    return m_createdAt;
}

void Payment::setCreatedAt(const QDateTime &createdAt)
{
    m_createdAt = createdAt;
}

QString Payment::studentNumber() const
{
    return m_studentNumber;
}

void Payment::setStudentNumber(const QString &studentNumber)
{
    m_studentNumber = studentNumber;
}
