#include "user.h"

/**
 * Default Constructor
 * Initializes a new user instance with default values.
 */
User::User() : m_id(0)
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the user
 * @param fullName - The full name of the user
 * @param username - The unique username for login
 * @param password - The password (hashed/encrypted)
 * @param role - The user's role (e.g., "admin", "professor", "student")
 */
User::User(int id, const QString& fullName, const QString& username, const QString& password, const QString& role)
{
    m_id = id;
    m_fullName = fullName;
    m_username = username;
    m_password = password;
    m_role = role;
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime();
}

/**
 * @return The unique ID of the user
 */
int User::id() const {
    return m_id;
}

/**
 * Sets the unique ID of the user
 * @param id - The new ID
 */
void User::setId(int id) {
    m_id = id;
}

/**
 * @return The full name of the user
 */
QString User::fullName() const {
    return m_fullName;
}

/**
 * Sets the full name of the user
 * @param fullName - The new full name
 */
void User::setFullName(const QString& fullName) {
    m_fullName = fullName;
}

/**
 * @return The username for login
 */
QString User::username() const {
    return m_username;
}

/**
 * Sets the username
 * @param username - The new username
 */
void User::setUsername(const QString& username) {
    m_username = username;
}

/**
 * @return The password of the user
 */
QString User::password() const {
    return m_password;
}

/**
 * Sets the password
 * @param password - The new password
 */
void User::setPassword(const QString& password) {
    m_password = password;
}

/**
 * @return The role of the user
 */
QString User::role() const {
    return m_role;
}

/**
 * Sets the role of the user
 * @param role - The new role
 */
void User::setRole(const QString& role) {
    m_role = role;
}

/**
 * @return The timestamp when the user account was created
 */
QDateTime User::createdAt() const {
    return m_createdAt;
}

/**
 * Sets the creation timestamp
 * @param createdAt - The new timestamp
 */
void User::setCreatedAt(const QDateTime& createdAt) {
    m_createdAt = createdAt;
}

/**
 * @return The timestamp when the user account was last updated
 */
QDateTime User::updatedAt() const {
    return m_updatedAt;
}

/**
 * Sets the update timestamp
 * @param updatedAt - The new timestamp
 */
void User::setUpdatedAt(const QDateTime& updatedAt) {
    m_updatedAt = updatedAt;
}
