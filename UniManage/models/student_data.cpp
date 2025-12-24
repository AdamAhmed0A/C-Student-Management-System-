#include "student_data.h"

/**
 * Default Constructor
 * Initializes a new student data instance with default values.
 */
StudentData::StudentData()
    : m_id(0), m_userId(0), m_departmentId(0), m_collegeId(0), m_academicLevelId(0), m_sectionId(0), m_tuitionFees(0.0),
      m_levelName(""), m_sectionName("")
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the student data
 * @param userId - The ID of the associated user account
 * @param studentNumber - The unique student number
 * @param idNumber - The student's national ID number
 * @param phone - The student's phone number
 * @param dob - The student's date of birth
 * @param department - The department name (legacy or display)
 * @param sectionId - The ID of the section the student is assigned to
 * @param seatNumber - The student's seat number (if applicable)
 * @param status - The student's academic status (e.g., "Active", "Suspended")
 */
StudentData::StudentData(int id, int userId, const QString &studentNumber, const QString &idNumber, const QString &phone, const QDateTime &dob, const QString &department, int sectionId, const QString &seatNumber, const QString &status)
{
    m_id = id;
    m_userId = userId;
    m_studentNumber = studentNumber;
    m_idNumber = idNumber;
    m_phone = phone;
    m_dob = dob;
    m_department = department;
    m_departmentId = 0;
    m_collegeId = 0;
    m_sectionId = sectionId;
    m_seatNumber = seatNumber;
    m_status = status;
    m_academicLevelId = 0;
    m_tuitionFees = 0.0;
    m_levelName = "";
    m_sectionName = "";
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime();
}

/**
 * @return The unique ID of the student data
 */
int StudentData::id() const { return m_id; }

/**
 * Sets the unique ID of the student data
 * @param id - The new ID
 */
void StudentData::setId(int id) { m_id = id; }

/**
 * @return The ID of the associated user account
 */
int StudentData::userId() const { return m_userId; }

/**
 * Sets the associated user ID
 * @param userId - The new user ID
 */
void StudentData::setUserId(int userId) { m_userId = userId; }

/**
 * @return The student's unique number (e.g., registration number)
 */
QString StudentData::studentNumber() const { return m_studentNumber; }

/**
 * Sets the student's unique number
 * @param studentNumber - The new student number
 */
void StudentData::setStudentNumber(const QString &studentNumber) { m_studentNumber = studentNumber; }

/**
 * @return The student's national ID number
 */
QString StudentData::idNumber() const { return m_idNumber; }

/**
 * Sets the student's national ID number
 * @param idNumber - The new ID number
 */
void StudentData::setIdNumber(const QString &idNumber) { m_idNumber = idNumber; }

/**
 * @return The student's phone number
 */
QString StudentData::phone() const { return m_phone; }

/**
 * Sets the student's phone number
 * @param phone - The new phone number
 */
void StudentData::setPhone(const QString &phone) { m_phone = phone; }

/**
 * @return The student's date of birth
 */
QDateTime StudentData::dob() const { return m_dob; }

/**
 * Sets the student's date of birth
 * @param dob - The new date of birth
 */
void StudentData::setDob(const QDateTime &dob) { m_dob = dob; }

/**
 * @return The student's department name
 */
QString StudentData::department() const { return m_department; }

/**
 * Sets the student's department name
 * @param department - The new department name
 */
void StudentData::setDepartment(const QString &department) { m_department = department; }

/**
 * @return The ID of the department the student belongs to
 */
int StudentData::departmentId() const { return m_departmentId; }

/**
 * Sets the department ID
 * @param id - The new department ID
 */
void StudentData::setDepartmentId(int id) { m_departmentId = id; }

/**
 * @return The ID of the academic level the student is in
 */
int StudentData::academicLevelId() const { return m_academicLevelId; }

/**
 * Sets the academic level ID
 * @param id - The new academic level ID
 */
void StudentData::setAcademicLevelId(int id) { m_academicLevelId = id; }

/**
 * @return The ID of the section the student is assigned to
 */
int StudentData::sectionId() const { return m_sectionId; }

/**
 * Sets the section ID
 * @param sectionId - The new section ID
 */
void StudentData::setSectionId(int sectionId) { m_sectionId = sectionId; }

/**
 * @return The student's seat number
 */
QString StudentData::seatNumber() const { return m_seatNumber; }

/**
 * Sets the student's seat number
 * @param seatNumber - The new seat number
 */
void StudentData::setSeatNumber(const QString &seatNumber) { m_seatNumber = seatNumber; }

/**
 * @return The student's academic status
 */
QString StudentData::status() const { return m_status; }

/**
 * Sets the student's academic status
 * @param status - The new status
 */
void StudentData::setStatus(const QString &status) { m_status = status; }

/**
 * @return The timestamp when the student data was created
 */
QDateTime StudentData::createdAt() const { return m_createdAt; }

/**
 * Sets the creation timestamp
 * @param createdAt - The new timestamp
 */
void StudentData::setCreatedAt(const QDateTime &createdAt) { m_createdAt = createdAt; }

/**
 * @return The timestamp when the student data was last updated
 */
QDateTime StudentData::updatedAt() const { return m_updatedAt; }

/**
 * Sets the update timestamp
 * @param updatedAt - The new timestamp
 */
void StudentData::setUpdatedAt(const QDateTime &updatedAt) { m_updatedAt = updatedAt; }

/**
 * @return The student's full name (derived/joined from User)
 */
QString StudentData::fullName() const { return m_fullName; }

/**
 * Sets the student's full name
 * @param fullName - The new full name
 */
void StudentData::setFullName(const QString &fullName) { m_fullName = fullName; }

/**
 * @return The student's username (derived/joined from User)
 */
QString StudentData::username() const { return m_username; }

/**
 * Sets the student's username
 * @param username - The new username
 */
void StudentData::setUsername(const QString &username) { m_username = username; }

/**
 * @return The user's role (derived/joined from User)
 */
QString StudentData::role() const { return m_role; }

/**
 * Sets the user's role
 * @param role - The new role
 */
void StudentData::setRole(const QString &role) { m_role = role; }

/**
 * @return The ID of the college the student belongs to
 */
int StudentData::collegeId() const { return m_collegeId; }

/**
 * Sets the college ID
 * @param collegeId - The new college ID
 */
void StudentData::setCollegeId(int collegeId) { m_collegeId = collegeId; }

/**
 * @return The name of the college (derived/joined)
 */
QString StudentData::collegeName() const { return m_collegeName; }

/**
 * Sets the college name
 * @param collegeName - The new college name
 */
void StudentData::setCollegeName(const QString &collegeName) { m_collegeName = collegeName; }

/**
 * @return The tuition fees applicable to the student
 */
double StudentData::tuitionFees() const { return m_tuitionFees; }

/**
 * Sets the tuition fees
 * @param fees - The new fees amount
 */
void StudentData::setTuitionFees(double fees) { m_tuitionFees = fees; }

/**
 * @return The name of the academic level (derived/joined)
 */
QString StudentData::levelName() const { return m_levelName; }

/**
 * Sets the academic level name
 * @param levelName - The new level name
 */
void StudentData::setLevelName(const QString &levelName) { m_levelName = levelName; }

/**
 * @return The name of the section (derived/joined)
 */
QString StudentData::sectionName() const { return m_sectionName; }

/**
 * Sets the section name
 * @param sectionName - The new section name
 */
void StudentData::setSectionName(const QString &sectionName) { m_sectionName = sectionName; }
