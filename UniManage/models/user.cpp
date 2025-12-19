#include "user.h"

User::User() : m_id(0)
{
}

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

// ID
int User::id() const {
    return m_id;
}

void User::setId(int id) {
    m_id = id;
}

// Full Name
QString User::fullName() const {
    return m_fullName;
}

void User::setFullName(const QString& fullName) {
    m_fullName = fullName;
}

// Username
QString User::username() const {
    return m_username;
}

void User::setUsername(const QString& username) {
    m_username = username;
}

// Password
QString User::password() const {
    return m_password;
}

void User::setPassword(const QString& password) {
    m_password = password;
}

// Role
QString User::role() const {
    return m_role;
}

void User::setRole(const QString& role) {
    m_role = role;
}

// Created At
QDateTime User::createdAt() const {
    return m_createdAt;
}

void User::setCreatedAt(const QDateTime& createdAt) {
    m_createdAt = createdAt;
}

// Updated At
QDateTime User::updatedAt() const {
    return m_updatedAt;
}

void User::setUpdatedAt(const QDateTime& updatedAt) {
    m_updatedAt = updatedAt;
}
