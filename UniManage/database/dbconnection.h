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
    bool createTables();
    bool insertDefaultData();

private:
    DBConnection();
    ~DBConnection();
    DBConnection(const DBConnection&) = delete;
    DBConnection& operator=(const DBConnection&) = delete;

    QSqlDatabase m_database;
    QString m_databasePath;
};

#endif // DBCONNECTION_H
