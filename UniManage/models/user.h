#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    User();
    User(int id, const QString& fullName, const QString& username, const QString& password, const QString& role);

    int id() const;
    void setId(int id);

    QString fullName() const;
    void setFullName(const QString& fullName);

    QString username() const;
    void setUsername(const QString& username);

    QString password() const;
    void setPassword(const QString& password);

    QString role() const;
    void setRole(const QString& role);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime& updatedAt);

private:
    int m_id;
    QString m_fullName;
    QString m_username;
    QString m_password;
    QString m_role;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
};

#endif // USER_H
