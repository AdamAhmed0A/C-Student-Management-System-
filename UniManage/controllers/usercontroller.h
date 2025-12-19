#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "../models/user.h"
#include <QList>
#include <QString>

class UserController
{
public:
    UserController();
    
    bool addUser(const User& user, QString* error = nullptr);
    bool updateUser(const User& user);
    bool deleteUser(int id);
    
    QList<User> getAllUsers();
    User getUserByUsername(const QString& username);
    User getUserById(int id);
    User authenticateUsingUsername(const QString& username, const QString& password);
};

#endif // USERCONTROLLER_H
