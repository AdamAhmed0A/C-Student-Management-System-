#ifndef USER_DAO_H
#define USER_DAO_H

#include "../UniManage/models/user.h"
#include <QList>
#include <QString>

class UserDAO
{
public:
    virtual ~UserDAO() = default;

    virtual bool insert(User& user) = 0;
    virtual bool update(const User& user) = 0;
    virtual bool remove(int id) = 0;
    virtual QList<User> getAll() = 0;
    virtual User getById(int id) = 0;
    virtual User getByUsername(const QString& username) = 0;
    virtual User authenticate(const QString& username, const QString& password) = 0;
};

#endif // USER_DAO_H