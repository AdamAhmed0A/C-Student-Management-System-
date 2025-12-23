#include "enrollment.h"

Enrollment::Enrollment()
    : m_id(0), m_studentId(0), m_courseId(0), m_status("active"),
      m_attendanceCount(0), m_absenceCount(0),
      m_assignment1Grade(0.0), m_assignment2Grade(0.0),
      m_courseworkGrade(0.0), m_finalExamGrade(0.0), m_experienceGrade(0.0),
      m_totalGrade(0.0), m_isRafaaApplied(false),
      m_courseMaxGrade(100), m_courseType("Theoretical")
{
    m_enrolledAt = QDateTime::currentDateTime();
}

int Enrollment::id() const { return m_id; }
void Enrollment::setId(int id) { m_id = id; }

int Enrollment::studentId() const { return m_studentId; }
void Enrollment::setStudentId(int studentId) { m_studentId = studentId; }

int Enrollment::courseId() const { return m_courseId; }
void Enrollment::setCourseId(int courseId) { m_courseId = courseId; }

QString Enrollment::status() const { return m_status; }
void Enrollment::setStatus(const QString& status) { m_status = status; }

int Enrollment::attendanceCount() const { return m_attendanceCount; }
void Enrollment::setAttendanceCount(int count) { m_attendanceCount = count; }

int Enrollment::absenceCount() const { return m_absenceCount; }
void Enrollment::setAbsenceCount(int count) { m_absenceCount = count; }

double Enrollment::assignment1Grade() const { return m_assignment1Grade; }
void Enrollment::setAssignment1Grade(double grade) { m_assignment1Grade = grade; }

double Enrollment::assignment2Grade() const { return m_assignment2Grade; }
void Enrollment::setAssignment2Grade(double grade) { m_assignment2Grade = grade; }

double Enrollment::courseworkGrade() const { return m_courseworkGrade; }
void Enrollment::setCourseworkGrade(double grade) { m_courseworkGrade = grade; }

double Enrollment::finalExamGrade() const { return m_finalExamGrade; }
void Enrollment::setFinalExamGrade(double grade) { m_finalExamGrade = grade; }

double Enrollment::experienceGrade() const { return m_experienceGrade; }
void Enrollment::setExperienceGrade(double grade) { m_experienceGrade = grade; }

double Enrollment::totalGrade() const { return m_totalGrade; }
void Enrollment::setTotalGrade(double grade) { m_totalGrade = grade; }

QString Enrollment::letterGrade() const { return m_letterGrade; }
void Enrollment::setLetterGrade(const QString& grade) { m_letterGrade = grade; }

bool Enrollment::isRafaaApplied() const { return m_isRafaaApplied; }
void Enrollment::setIsRafaaApplied(bool applied) { m_isRafaaApplied = applied; }

QDateTime Enrollment::enrolledAt() const { return m_enrolledAt; }
void Enrollment::setEnrolledAt(const QDateTime& enrolledAt) { m_enrolledAt = enrolledAt; }

QString Enrollment::studentName() const { return m_studentName; }
void Enrollment::setStudentName(const QString& name) { m_studentName = name; }

QString Enrollment::courseName() const { return m_courseName; }
void Enrollment::setCourseName(const QString& name) { m_courseName = name; }

int Enrollment::courseMaxGrade() const { return m_courseMaxGrade; }
void Enrollment::setCourseMaxGrade(int maxGrade) { m_courseMaxGrade = maxGrade; }

QString Enrollment::courseType() const { return m_courseType; }
void Enrollment::setCourseType(const QString& type) { m_courseType = type; }

QString Enrollment::studentSection() const { return m_studentSection; }
void Enrollment::setStudentSection(const QString& section) { m_studentSection = section; }

QString Enrollment::studentLevel() const { return m_studentLevel; }
void Enrollment::setStudentLevel(const QString& level) { m_studentLevel = level; }

QString Enrollment::studentCode() const { return m_studentCode; }
void Enrollment::setStudentCode(const QString& code) { m_studentCode = code; }

QString Enrollment::academicYear() const { return m_academicYear; }
void Enrollment::setAcademicYear(const QString& year) { m_academicYear = year; }
