#include "professor.h"

/**
 * Default Constructor
 * Initializes a new professor instance with default values (id=0, userId=0)
 */
Professor::Professor() : m_id(0), m_userId(0) {}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the professor data
 * @param userId - The ID of the associated user account
 * @param specialization - The professor's field of expertise
 * @param title - The professor's academic title (e.g., "Dr.", "Prof.")
 */
Professor::Professor(int id, int userId, const QString& specialization, const QString& title)
    : m_id(id), m_userId(userId), m_specialization(specialization), m_title(title) {}

/**
 * @return The unique ID of the professor data
 */
int Professor::id() const { return m_id; }

/**
 * Sets the unique ID of the professor data
 * @param id - The new ID
 */
void Professor::setId(int id) { m_id = id; }

/**
 * @return The ID of the associated user account
 */
int Professor::userId() const { return m_userId; }

/**
 * Sets the associated user ID
 * @param userId - The new user ID
 */
void Professor::setUserId(int userId) { m_userId = userId; }

/**
 * @return The professor's specialization
 */
QString Professor::specialization() const { return m_specialization; }

/**
 * Sets the professor's specialization
 * @param specialization - The new specialization
 */
void Professor::setSpecialization(const QString& specialization) { m_specialization = specialization; }

/**
 * @return The professor's academic title
 */
QString Professor::title() const { return m_title; }

/**
 * Sets the professor's academic title
 * @param title - The new title
 */
void Professor::setTitle(const QString& title) { m_title = title; }

/**
 * @return Additional personal information/bio
 */
QString Professor::personalInfo() const { return m_personalInfo; }

/**
 * Sets additional personal information
 * @param info - The new info text
 */
void Professor::setPersonalInfo(const QString& info) { m_personalInfo = info; }

/**
 * @return The professor's government/national ID number
 */
QString Professor::idNumber() const { return m_idNumber; }

/**
 * Sets the professor's identification number
 * @param idNumber - The new ID number
 */
void Professor::setIdNumber(const QString& idNumber) { m_idNumber = idNumber; }

/**
 * @return The professor's full name (derived/joined from Users table)
 */
QString Professor::fullName() const { return m_fullName; }

/**
 * Sets the professor's full name
 * @param name - The full name
 */
void Professor::setFullName(const QString& name) { m_fullName = name; }

QString Professor::username() const { return m_username; }

void Professor::setUsername(const QString& username) { m_username = username; }
