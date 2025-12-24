#include "department.h"

/**
 * Default Constructor
 * Initializes a new instance with default values (id=0, collegeId=0)
 */
Department::Department() : m_id(0), m_collegeId(0) {}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the department
 * @param name - The name of the department (e.g., "Computer Science")
 * @param collegeId - The ID of the college this department belongs to
 * @param code - A short code for the department (e.g., "CS")
 */
Department::Department(int id, const QString& name, int collegeId, const QString& code)
    : m_id(id), m_name(name), m_collegeId(collegeId), m_code(code) {}

/**
 * @return The unique ID of the department
 */
int Department::id() const { return m_id; }

/**
 * Sets the unique ID of the department
 * @param id - The new ID
 */
void Department::setId(int id) { m_id = id; }

/**
 * @return The name of the department
 */
QString Department::name() const { return m_name; }

/**
 * Sets the name of the department
 * @param name - The new name
 */
void Department::setName(const QString& name) { m_name = name; }

/**
 * @return The ID of the college the department belongs to
 */
int Department::collegeId() const { return m_collegeId; }

/**
 * Sets the college ID for the department
 * @param collegeId - The new college ID
 */
void Department::setCollegeId(int collegeId) { m_collegeId = collegeId; }

/**
 * @return The short code of the department
 */
QString Department::code() const { return m_code; }

/**
 * Sets the short code of the department
 * @param code - The new code
 */
void Department::setCode(const QString& code) { m_code = code; }

/**
 * @return The name of the college (derived/joined)
 */
QString Department::collegeName() const { return m_collegeName; }

/**
 * Sets the college name for display purposes
 * @param name - The college name
 */
void Department::setCollegeName(const QString& name) { m_collegeName = name; }
