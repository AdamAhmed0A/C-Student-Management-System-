#include "enrollment.h"

/**
 * Default Constructor
 * Initializes a new enrollment instance with default values
 * Status="active", grades=0.0, counts=0, maxGrade=100, type="Theoretical"
 */
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

/**
 * @return The unique ID of the enrollment
 */
int Enrollment::id() const { return m_id; }

/**
 * Sets the unique ID of the enrollment
 * @param id - The new ID
 */
void Enrollment::setId(int id) { m_id = id; }

/**
 * @return The ID of the student enrolled
 */
int Enrollment::studentId() const { return m_studentId; }

/**
 * Sets the student's ID
 * @param studentId - The new student ID
 */
void Enrollment::setStudentId(int studentId) { m_studentId = studentId; }

/**
 * @return The ID of the course enrolled in
 */
int Enrollment::courseId() const { return m_courseId; }

/**
 * Sets the course's ID
 * @param courseId - The new course ID
 */
void Enrollment::setCourseId(int courseId) { m_courseId = courseId; }

/**
 * @return The enrollment status (e.g., "active", "completed", "dropped")
 */
QString Enrollment::status() const { return m_status; }

/**
 * Sets the enrollment status
 * @param status - The new status
 */
void Enrollment::setStatus(const QString& status) { m_status = status; }

/**
 * @return The number of classes attended
 */
int Enrollment::attendanceCount() const { return m_attendanceCount; }

/**
 * Sets the number of classes attended
 * @param count - The attendance count
 */
void Enrollment::setAttendanceCount(int count) { m_attendanceCount = count; }

/**
 * @return The number of classes absent
 */
int Enrollment::absenceCount() const { return m_absenceCount; }

/**
 * Sets the number of classes absent
 * @param count - The absence count
 */
void Enrollment::setAbsenceCount(int count) { m_absenceCount = count; }

/**
 * @return The grade for Assignment 1
 */
double Enrollment::assignment1Grade() const { return m_assignment1Grade; }

/**
 * Sets the grade for Assignment 1
 * @param grade - The grade value
 */
void Enrollment::setAssignment1Grade(double grade) { m_assignment1Grade = grade; }

/**
 * @return The grade for Assignment 2
 */
double Enrollment::assignment2Grade() const { return m_assignment2Grade; }

/**
 * Sets the grade for Assignment 2
 * @param grade - The grade value
 */
void Enrollment::setAssignment2Grade(double grade) { m_assignment2Grade = grade; }

/**
 * @return The grade for other coursework
 */
double Enrollment::courseworkGrade() const { return m_courseworkGrade; }

/**
 * Sets the grade for other coursework
 * @param grade - The grade value
 */
void Enrollment::setCourseworkGrade(double grade) { m_courseworkGrade = grade; }

/**
 * @return The grade for the final exam
 */
double Enrollment::finalExamGrade() const { return m_finalExamGrade; }

/**
 * Sets the grade for the final exam
 * @param grade - The grade value
 */
void Enrollment::setFinalExamGrade(double grade) { m_finalExamGrade = grade; }

/**
 * @return The grade for practical experience/labs
 */
double Enrollment::experienceGrade() const { return m_experienceGrade; }

/**
 * Sets the grade for practical experience/labs
 * @param grade - The grade value
 */
void Enrollment::setExperienceGrade(double grade) { m_experienceGrade = grade; }

/**
 * @return The calculated total grade
 */
double Enrollment::totalGrade() const { return m_totalGrade; }

/**
 * Sets the total grade
 * @param grade - The total grade value
 */
void Enrollment::setTotalGrade(double grade) { m_totalGrade = grade; }

/**
 * @return The letter grade (e.g., "A", "B", "C")
 */
QString Enrollment::letterGrade() const { return m_letterGrade; }

/**
 * Sets the letter grade
 * @param grade - The letter grade
 */
void Enrollment::setLetterGrade(const QString& grade) { m_letterGrade = grade; }

/**
 * @return True if "Rafaa" (grade boosting/mercy) has been applied
 */
bool Enrollment::isRafaaApplied() const { return m_isRafaaApplied; }

/**
 * Sets the "Rafaa" application status
 * @param applied - True if applied
 */
void Enrollment::setIsRafaaApplied(bool applied) { m_isRafaaApplied = applied; }

/**
 * @return The date and time of enrollment
 */
QDateTime Enrollment::enrolledAt() const { return m_enrolledAt; }

/**
 * Sets the enrollment timestamp
 * @param enrolledAt - The new timestamp
 */
void Enrollment::setEnrolledAt(const QDateTime& enrolledAt) { m_enrolledAt = enrolledAt; }

/**
 * @return The name of the student (derived/joined)
 */
QString Enrollment::studentName() const { return m_studentName; }

/**
 * Sets the student name
 * @param name - The student name
 */
void Enrollment::setStudentName(const QString& name) { m_studentName = name; }

/**
 * @return The name of the course (derived/joined)
 */
QString Enrollment::courseName() const { return m_courseName; }

/**
 * Sets the course name
 * @param name - The course name
 */
void Enrollment::setCourseName(const QString& name) { m_courseName = name; }

/**
 * @return The max grade for the course (for calculation)
 */
int Enrollment::courseMaxGrade() const { return m_courseMaxGrade; }

/**
 * Sets the max grade for the course
 * @param maxGrade - The new max grade
 */
void Enrollment::setCourseMaxGrade(int maxGrade) { m_courseMaxGrade = maxGrade; }

/**
 * @return The type of the course ("Theoretical" vs "Practical")
 */
QString Enrollment::courseType() const { return m_courseType; }

/**
 * Sets the type of the course
 * @param type - The new course type
 */
void Enrollment::setCourseType(const QString& type) { m_courseType = type; }

/**
 * @return The student's section/group (e.g., "Group A")
 */
QString Enrollment::studentSection() const { return m_studentSection; }

/**
 * Sets the student's section/group
 * @param section - The section name
 */
void Enrollment::setStudentSection(const QString& section) { m_studentSection = section; }

/**
 * @return The student's academic level/year (e.g., "Year 1")
 */
QString Enrollment::studentLevel() const { return m_studentLevel; }

/**
 * Sets the student's academic level/year
 * @param level - The level name
 */
void Enrollment::setStudentLevel(const QString& level) { m_studentLevel = level; }

/**
 * @return The student's unique code identifier
 */
QString Enrollment::studentCode() const { return m_studentCode; }

/**
 * Sets the student's unique code
 * @param code - The student code
 */
void Enrollment::setStudentCode(const QString& code) { m_studentCode = code; }

/**
 * @return The academic year for this enrollment (e.g., "2023-2024")
 */
QString Enrollment::academicYear() const { return m_academicYear; }

/**
 * Sets the academic year for this enrollment
 * @param year - The academic year
 */
void Enrollment::setAcademicYear(const QString& year) { m_academicYear = year; }
