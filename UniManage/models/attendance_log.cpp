#include "attendance_log.h"

/**
 * Default Constructor
 * Initializes a new instance with default values (id=0, enrollmentId=0)
 */
AttendanceLog::AttendanceLog() : m_id(0), m_enrollmentId(0) {}

/**
 * @return The unique ID of the attendance log
 */
int AttendanceLog::id() const { return m_id; }

/**
 * Sets the unique ID of the attendance log
 * @param id - The new ID
 */
void AttendanceLog::setId(int id) { m_id = id; }

/**
 * @return The ID of the enrollment this log belongs to
 */
int AttendanceLog::enrollmentId() const { return m_enrollmentId; }

/**
 * Sets the ID of the related enrollment
 * @param enrollmentId - The enrollment ID
 */
void AttendanceLog::setEnrollmentId(int enrollmentId) { m_enrollmentId = enrollmentId; }

/**
 * @return The date of the attendance record
 */
QDate AttendanceLog::date() const { return m_date; }

/**
 * Sets the date of the attendance record
 * @param date - The date
 */
void AttendanceLog::setDate(const QDate& date) { m_date = date; }

/**
 * @return The status of attendance (e.g., "Present", "Absent")
 */
QString AttendanceLog::status() const { return m_status; }

/**
 * Sets the status of attendance
 * @param status - The new status
 */
void AttendanceLog::setStatus(const QString& status) { m_status = status; }

/**
 * @return Optional notes or remarks about the attendance
 */
QString AttendanceLog::notes() const { return m_notes; }

/**
 * Sets optional notes for the attendance record
 * @param notes - The new notes
 */
void AttendanceLog::setNotes(const QString& notes) { m_notes = notes; }
