#include "attendance_log.h"

AttendanceLog::AttendanceLog() : m_id(0), m_enrollmentId(0) {}

int AttendanceLog::id() const { return m_id; }
void AttendanceLog::setId(int id) { m_id = id; }

int AttendanceLog::enrollmentId() const { return m_enrollmentId; }
void AttendanceLog::setEnrollmentId(int enrollmentId) { m_enrollmentId = enrollmentId; }

QDate AttendanceLog::date() const { return m_date; }
void AttendanceLog::setDate(const QDate& date) { m_date = date; }

QString AttendanceLog::status() const { return m_status; }
void AttendanceLog::setStatus(const QString& status) { m_status = status; }

QString AttendanceLog::notes() const { return m_notes; }
void AttendanceLog::setNotes(const QString& notes) { m_notes = notes; }
