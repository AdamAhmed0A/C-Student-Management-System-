#include "section.h"

/**
 * Default Constructor
 * Initializes a new section with default values
 * Defaults: m_id=0, m_capacity=40
 */
Section::Section()
    : m_id(0), m_name(""), m_courseId(0), m_capacity(40), m_semesterId(0), m_academicLevelId(0)
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the section
 * @param name - The name of the section (e.g., "Group A")
 * @param courseId - The ID of the associated course
 * @param capacity - The maximum student capacity
 * @param semesterId - The ID of the semester
 */
Section::Section(int id, const QString& name, int courseId, int capacity, int semesterId)
{
    m_id = id;
    m_name = name;
    m_courseId = courseId;
    m_capacity = capacity;
    m_semesterId = semesterId;
    m_academicLevelId = 0; // Initialize explicitly
}

/**
 * @return The name of the section
 */
QString Section::name() const {
    return m_name;
}

/**
 * Sets the name of the section
 * @param name - The new name
 */
void Section::setName(const QString& name) {
    m_name = name;
}

/**
 * @return The unique ID of the section
 */
int Section::id() const {
    return m_id;
}

/**
 * Sets the unique ID of the section
 * @param id - The new ID
 */
void Section::setId(int id) {
    m_id = id;
}

/**
 * @return The ID of the associated course
 */
int Section::courseId() const {
    return m_courseId;
}

/**
 * Sets the associated course ID
 * @param courseId - The new course ID
 */
void Section::setCourseId(int courseId) {
    m_courseId = courseId;
}

/**
 * @return The student capacity of the section
 */
int Section::capacity() const {
    return m_capacity;
}

/**
 * Sets the student capacity
 * @param capacity - The new capacity
 */
void Section::setCapacity(int capacity) {
    m_capacity = capacity;
}

/**
 * @return The ID of the semester
 */
int Section::semesterId() const {
    return m_semesterId;
}

/**
 * Sets the semester ID
 * @param semesterId - The new semester ID
 */
void Section::setSemesterId(int semesterId) {
    m_semesterId = semesterId;
}

/**
 * @return The timestamp when the section was created
 */
QDateTime Section::createdAt() const {
    return m_createdAt;
}

/**
 * Sets the creation timestamp
 * @param createdAt - The new timestamp
 */
void Section::setCreatedAt(const QDateTime& createdAt) {
    m_createdAt = createdAt;
}

/**
 * @return The timestamp when the section was last updated
 */
QDateTime Section::updatedAt() const {
    return m_updatedAt;
}

/**
 * Sets the update timestamp
 * @param updatedAt - The new timestamp
 */
void Section::setUpdatedAt(const QDateTime& updatedAt) {
    m_updatedAt = updatedAt;
}

/**
 * @return The name of the course (derived/joined)
 */
QString Section::courseName() const {
    return m_courseName;
}

/**
 * Sets the course name
 * @param courseName - The course name
 */
void Section::setCourseName(const QString& courseName) {
    m_courseName = courseName;
}

/**
 * @return The ID of the academic level this section belongs to
 */
int Section::academicLevelId() const {
    return m_academicLevelId;
}

/**
 * Sets the academic level ID
 * @param levelId - The new level ID
 */
void Section::setAcademicLevelId(int levelId) {
    m_academicLevelId = levelId;
}
