#include "payment.h"

/**
 * Default Constructor
 * Initializes a new payment record with default values
 * Defaults: m_id=0, m_studentId=0, m_amount=0.0f
 */
Payment::Payment()
    : m_id(0), m_studentId(0), m_amount(0.0f)
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the payment
 * @param studentId - The ID of the student making the payment
 * @param amount - The payment amount
 * @param date - The date of the payment transaction
 * @param year - The academic year associated with the payment
 * @param method - The payment method (e.g., "Cash", "Card")
 * @param status - The payment status (e.g., "Paid", "Pending")
 * @param notes - Optional notes or remarks
 * @param createdAt - When the record was created
 * @param studentNumber - The student's unique number (for reference/joining)
 */
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

/**
 * @return The unique ID of the payment
 */
int Payment::id() const
{
    return m_id;
}

/**
 * Sets the unique ID of the payment
 * @param id - The new ID
 */
void Payment::setId(int id)
{
    m_id = id;
}

/**
 * @return The ID of the student making the payment
 */
int Payment::studentId() const
{
    return m_studentId;
}

/**
 * Sets the student's ID
 * @param studentId - The new student ID
 */
void Payment::setStudentId(int studentId)
{
    m_studentId = studentId;
}

/**
 * @return The amount paid
 */
float Payment::amount() const
{
    return m_amount;
}

/**
 * Sets the payment amount
 * @param amount - The new amount
 */
void Payment::setAmount(float amount)
{
    m_amount = amount;
}

/**
 * @return The date of the payment transaction
 */
QDateTime Payment::date() const
{
    return m_date;
}

/**
 * Sets the payment transaction date
 * @param date - The new date
 */
void Payment::setDate(const QDateTime &date)
{
    m_date = date;
}

/**
 * @return The academic year associated with the payment
 */
QDateTime Payment::year() const
{
    return m_year;
}

/**
 * Sets the academic year
 * @param year - The new year
 */
void Payment::setYear(const QDateTime &year)
{
    m_year = year;
}

/**
 * @return The payment method (e.g., "Cash", "Online")
 */
QString Payment::method() const
{
    return m_method;
}

/**
 * Sets the payment method
 * @param method - The new method
 */
void Payment::setMethod(const QString &method)
{
    m_method = method;
}

/**
 * @return The payment status (e.g., "Paid", "Pending")
 */
QString Payment::status() const
{
    return m_status;
}

/**
 * Sets the payment status
 * @param status - The new status
 */
void Payment::setStatus(const QString &status)
{
    m_status = status;
}

/**
 * @return Additional notes or remarks
 */
QString Payment::notes() const
{
    return m_notes;
}

/**
 * Sets additional notes for the payment
 * @param notes - The notes content
 */
void Payment::setNotes(const QString &notes)
{
    m_notes = notes;
}

/**
 * @return The timestamp when the record was created
 */
QDateTime Payment::createdAt() const
{
    return m_createdAt;
}

/**
 * Sets the creation timestamp
 * @param createdAt - The creation date and time
 */
void Payment::setCreatedAt(const QDateTime &createdAt)
{
    m_createdAt = createdAt;
}

/**
 * @return The student number associated with the payment
 */
QString Payment::studentNumber() const
{
    return m_studentNumber;
}

/**
 * Sets the student number
 * @param studentNumber - The student number
 */
void Payment::setStudentNumber(const QString &studentNumber)
{
    m_studentNumber = studentNumber;
}
