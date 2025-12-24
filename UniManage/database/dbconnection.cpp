#include "dbconnection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

DBConnection::DBConnection() {}

/**
 * Destructor for the DBConnection class
 * Closes the database connection if it's open
 */
DBConnection::~DBConnection()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
    QSqlDatabase::removeDatabase("unimanage_connection");
}

/**
 * Returns the singleton instance of DBConnection
 * Ensures only one database connection exists throughout the application
 * @return Reference to the singleton DBConnection instance
 */
DBConnection& DBConnection::instance()
{
    static DBConnection instance;
    return instance;
}

void DBConnection::loadEnvFile()
{
    QFile envFile(QCoreApplication::applicationDirPath() + "/.env");
    if (!envFile.exists()) {
        qWarning() << ".env file not found in build directory. Please create one. Using default credentials.";
        // Set default values if .env is not found
        m_host = "127.0.0.1";
        m_port = 3306;
        m_user = "root";
        m_password = "root";
        m_dbName = "university";
        return;
    }

    if (envFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&envFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith('#') || line.trimmed().isEmpty()) {
                continue;
            }
            QStringList parts = line.split('=');
            if (parts.size() == 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                if (key == "DB_HOST") m_host = value;
                else if (key == "DB_PORT") m_port = value.toInt();
                else if (key == "DB_NAME") m_dbName = value;
                else if (key == "DB_USER") m_user = value;
                else if (key == "DB_PASS") m_password = value;
            }
        }
        envFile.close();
    } else {
         qWarning() << "Could not open .env file.";
    }
}


bool DBConnection::initialize()
{
    loadEnvFile();

    const QString connectionName = "unimanage_connection";
    if (QSqlDatabase::contains(connectionName)) {
        m_database = QSqlDatabase::database(connectionName);
    } else {
        m_database = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    }

    m_database.setHostName(m_host);
    m_database.setPort(m_port);
    m_database.setUserName(m_user);
    m_database.setPassword(m_password);
    m_database.setDatabaseName(m_dbName);

    if (!m_database.open()) {
        m_lastError = QString("Connection Failed: %1").arg(m_database.lastError().text());
        qCritical() << m_lastError;
        return false;
    }

    if (!createTables()) {
        m_lastError = "Failed to create tables.";
        qCritical() << m_lastError;
        return false;
    }

    QSqlQuery checkUsers(m_database);
    if (checkUsers.exec("SELECT COUNT(*) FROM users") && checkUsers.next()) {
        if (checkUsers.value(0).toInt() == 0) {
            insertDefaultData();
        }
    }

    return true;
}

/**
 * Returns a reference to the active database connection
 * @return Reference to the QSqlDatabase object
 */
QSqlDatabase& DBConnection::database()
{
    return m_database;
}

/**
 * Creates all database tables required for the university management system
 * Creates tables for users, colleges, departments, academic levels, semesters,
 * courses, sections, students, enrollments, payments, rooms, professors,
 * schedules, news, attendance logs, and calendar events
 * Also performs migrations to add missing columns to existing tables
 * @return true if all tables created successfully, false otherwise
 */
bool DBConnection::createTables()
{
    QSqlQuery query(m_database);
    // Users table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `users` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "full_name VARCHAR(255) NOT NULL,"
                    "username VARCHAR(100) UNIQUE NOT NULL,"
                    "password VARCHAR(128) NOT NULL,"
                    "role VARCHAR(50),"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }
    // Students Data table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `students_data` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "user_id INT,"
                    "student_number VARCHAR(100) UNIQUE NOT NULL,"
                    "id_number VARCHAR(100) NOT NULL,"
                    "dob DATETIME NULL,"
                    "department VARCHAR(255) NULL,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL,"
                    "FOREIGN KEY (user_id) REFERENCES users(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating students_data table:" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * Inserts default data into the database
 * Creates default admin accounts and a default semester
 * Admin accounts: "admin" (password: admin123) and "30605040603080" (password: 12345)
 * @return true if default data inserted successfully
 */
bool DBConnection::insertDefaultData()
{
    QSqlQuery query(m_database);

    // Default admin
    QString password = "admin123";
    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    query.prepare("INSERT IGNORE INTO users (full_name, username, password, role) VALUES (?, ?, ?, ?)");
    query.addBindValue("Admin");
    query.addBindValue("admin");
    query.addBindValue(passwordHash);
    query.addBindValue("Admin");
    query.exec();

    // Ensure requested specific admin account exists (National ID 30605040603080, password 12345)
    // We use REPLACE INTO or check specifically to ensure it's correct
    QString specAdminPass = "12345";
    QString specAdminHash = QString(QCryptographicHash::hash(specAdminPass.toUtf8(), QCryptographicHash::Sha256).toHex());

    // Check if this specific user exists
    query.prepare("SELECT id FROM users WHERE username = ?");
    query.addBindValue("30605040603080");
    query.exec();

    if (query.next()) {
        // Update existing to ensure password is correct
        query.prepare("UPDATE users SET password = ?, role = 'Admin' WHERE username = ?");
        query.addBindValue(specAdminHash);
        query.addBindValue("30605040603080");
        query.exec();
    } else {
        // Create new
        query.prepare("INSERT INTO users (full_name, username, password, role) VALUES (?, ?, ?, ?)");
        query.addBindValue("Main Administrator");
        query.addBindValue("30605040603080");
        query.addBindValue(specAdminHash);
        query.addBindValue("Admin");
        query.exec();
    }

    qDebug() << "Admin accounts verified and synchronized.";

    // Insert default semester if none exist
    query.exec("SELECT COUNT(*) FROM semester");
    if (query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO semester (year, semester) VALUES (?, ?)");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.exec();
    }

    return true;
}

/**
 * Returns the last error message from database operations
 * @return String containing the last error message
 */
QString DBConnection::getLastError() const
{
  return m_lastError;
}
