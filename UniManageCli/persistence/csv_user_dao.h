#ifndef CSV_USER_DAO_H
#define CSV_USER_DAO_H

#include "../../UniManage/models/user.h"
#include "dao/user_dao.h"
#include <QList>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCryptographicHash>

class CsvUserDAO : public UserDAO
{
public:
    CsvUserDAO(const QString& filePath);
    ~CsvUserDAO() override = default;

    bool insert(User& user) override;
    bool update(const User& user) override;
    bool remove(int id) override;
    QList<User> getAll() override;
    User getById(int id) override;
    User getByUsername(const QString& username) override;
    User authenticate(const QString& username, const QString& password) override;

private:
    QString m_filePath;
    QList<User> m_users;
    int m_nextId;

    void loadData();
    bool saveData();
    bool ensureFileExists();
    QStringList getExpectedHeaders() const;
    User parseLine(const QString& line) const;
    QString userToCsvLine(const User& user) const;
    QDateTime parseDateTime(const QString& str) const;
    QString dateTimeToString(const QDateTime& dt) const;
    QString hashPassword(const QString& password) const;
};

#endif
