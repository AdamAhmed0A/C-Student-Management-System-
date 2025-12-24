#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QSqlDatabase>
#include <QString>

class DBConnection
{
public:
    static DBConnection& instance();
    bool initialize();
    QSqlDatabase& database();
    QString getLastError() const;

private:
    DBConnection();
    ~DBConnection();
    DBConnection(const DBConnection&) = delete;
    DBConnection& operator=(const DBConnection&) = delete;

    void loadEnvFile();
    bool createTables();
    bool insertDefaultData();


    QSqlDatabase m_database;
    QString m_lastError;
    QString m_host;
    int m_port;
    QString m_user;
    QString m_password;
    QString m_dbName;
};

#endif // DBCONNECTION_H
