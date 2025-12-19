#include "usercontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

UserController::UserController() {}

// function to add user
bool UserController::addUser(const User& user)
{
    QSqlQuery query(DBConnection::instance().database());
	query.prepare(Queries::INSERT_USER);
    query.addBindValue(user.fullName());
    query.addBindValue(user.username());
    query.addBindValue(user.password());
    query.addBindValue(user.role());
    if (!query.exec()) {
        qDebug() << "Error adding user:" << query.lastError().text();
        return false;
    }
	return true;
}

// function to update user
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

//function to authenticate user using username and password
User UserController::authenticateUsingUsername(const QString& username, const QString& password)
{
    User u = getUserByUsername(username);
    if (u.id() != 0 && u.password() == password) {
        return u;
    }
    return User(); 
}
