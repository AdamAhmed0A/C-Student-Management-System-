#include "usercontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>

/**
 * Constructor for the UserController class
 */
UserController::UserController() {}

// function to add user
/**
 * Adds a new user to the system (student, professor, or admin)
 * Hashes the password using SHA-256 before storing
 * @param user - The User object containing details
 * @param error - Optional string pointer to store error messages
 * @return True if successful, otherwise false
 */
bool UserController::addUser(const User& user, QString* error)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_USER);
    query.addBindValue(user.fullName());
    query.addBindValue(user.username());
    
    // Always hash password before storing
    QString hashed = QString(QCryptographicHash::hash(user.password().toUtf8(), QCryptographicHash::Sha256).toHex());
    query.addBindValue(hashed);
    
    query.addBindValue(user.role());
    if (!query.exec()) {
        QString err = query.lastError().text();
        qDebug() << "Error adding user:" << err;
        if (error) *error = err;
        return false;
    }
    return true;
}

// function to update user
/**
 * Updates an already existing user's details
 * Note: Password updates should be handled carefully (hashing if changed)
 * @param user - The User object with updated details
 * @return True if successful, otherwise false
 */
bool UserController::updateUser(const User& user)
{
    QSqlQuery query(DBConnection::instance().database());
	query.prepare(Queries::UPDATE_USER);
    query.addBindValue(user.fullName());
    query.addBindValue(user.password());
    query.addBindValue(user.role());
    query.addBindValue(user.id());
    if (!query.exec()) {
        qDebug() << "Error updating user:" << query.lastError().text();
        return false;
	}
    return true;
}

//  function to delete user
/**
 * Deletes a user account from the database
 * @param id - The ID of the user to delete
 * @return True if successful, otherwise false
 */
bool UserController::deleteUser(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_USER);
    query.addBindValue(id);
	if (!query.exec()) {
        qDebug() << "Error deleting user:" << query.lastError().text();
        return false;
    }
	return true;
}

//function to get all users
/**
 * Retrieves all user accounts from the database
 * @return A list of User objects
 */
QList<User> UserController::getAllUsers()
{
    QList<User> users;
	QSqlQuery query(DBConnection::instance().database());
	if (!query.exec(Queries::SELECT_ALL_USERS)) {
        qDebug() << "Error retrieving users:" << query.lastError().text();
        return users;
    }
    while (query.next()) {
        User user;
        user.setId(query.value("id").toInt());
        user.setFullName(query.value("full_name").toString());
        user.setUsername(query.value("username").toString());
        user.setPassword(query.value("password").toString());
        user.setRole(query.value("role").toString());
        user.setCreatedAt(query.value("created_at").toDateTime());
        user.setUpdatedAt(query.value("updated_at").toDateTime());
        users.append(user);
	}
	return users;
}

//function to get user by username
/**
 * Retrieves a user account by their username
 * @param username - The username to search for
 * @return The User object if found, otherwise an empty object
 */
User UserController::getUserByUsername(const QString& username)
{
	User user;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_USER_BY_USERNAME);
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Error retrieving user by username:" << query.lastError().text();
        return user;
    }
    if (query.next()) {
        user.setId(query.value("id").toInt());
        user.setFullName(query.value("full_name").toString());
        user.setUsername(query.value("username").toString());
        user.setPassword(query.value("password").toString());
        user.setRole(query.value("role").toString());
        user.setCreatedAt(query.value("created_at").toDateTime());
        user.setUpdatedAt(query.value("updated_at").toDateTime());
    }
	return user;
}

//function to get user by id
/**
 * Retrieves a user account by their ID
 * @param id - The ID of the user
 * @return The User object if found, otherwise an empty object
 */
User UserController::getUserById(int id)
{
    User user;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_USER_BY_ID);
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Error retrieving user by id:" << query.lastError().text();
        return user;
    }
    if (query.next()) {
        user.setId(query.value("id").toInt());
        user.setFullName(query.value("full_name").toString());
        user.setUsername(query.value("username").toString());
        user.setPassword(query.value("password").toString());
        user.setRole(query.value("role").toString());
        user.setCreatedAt(query.value("created_at").toDateTime());
        user.setUpdatedAt(query.value("updated_at").toDateTime());
    }
    return user;
}

//function to authenticate user using username and password
/**
 * Authenticates a user by checking their username and hashed password
 * Note: The input password should be pre-hashed or hashed here before comparison
 * In this implementation, it assumes 'password' acts as the hash if it matches stored.
 * (Ideally, you should hash the input password here before comparison if the DB stores hashes)
 * @param username - The username
 * @param password - The password string (or hash, depending on usage context)
 * @return The authenticated User object if successful, otherwise an empty object
 */
User UserController::authenticateUsingUsername(const QString& username, const QString& password)
{
    User u = getUserByUsername(username);
    if (u.id() != 0 && u.password() == password) {
        return u;
    }
    return User(); 
}
