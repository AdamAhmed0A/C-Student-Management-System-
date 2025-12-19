#include "dbconnection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QProcessEnvironment>

DBConnection::DBConnection()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_databasePath = appDataPath + "/university.db";
    qDebug() << "Database file path:" << m_databasePath;

    // Read MySQL connection parameters from environment or use defaults
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_host = env.value("DB_HOST", "127.0.0.1");
    m_port = env.value("DB_PORT", "3306").toInt();
    m_user = env.value("DB_USER", "root");
    m_password = env.value("DB_PASSWORD", "root");
    m_dbName = env.value("DB_NAME", "university");
    qDebug() << "DB host:" << m_host << "port:" << m_port << "user:" << m_user << "db:" << m_dbName;
}

DBConnection::~DBConnection()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

DBConnection& DBConnection::instance()
{
    static DBConnection instance;
    return instance;
}

bool DBConnection::initialize()
{
    // Try to use MySQL
    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName(m_host);
    m_database.setPort(m_port);
    m_database.setUserName(m_user);
    m_database.setPassword(m_password);
    m_database.setDatabaseName(m_dbName);

    if (!m_database.open()) {
        qDebug() << "Initial MySQL open failed:" << m_database.lastError().text();

        // Try to create the database if it doesn't exist by connecting without a DB
        QString tempConnName = "__create_db_conn__";
        QSqlDatabase tmp = QSqlDatabase::addDatabase("QMYSQL", tempConnName);
        tmp.setHostName(m_host);
        tmp.setPort(m_port);
        tmp.setUserName(m_user);
        tmp.setPassword(m_password);

        if (!tmp.open()) {
            qDebug() << "Error: unable to connect to MySQL server -" << tmp.lastError().text();
            QSqlDatabase::removeDatabase(tempConnName);
            return false;
        }

        QSqlQuery q(tmp);
        QString createdb = QString("CREATE DATABASE IF NOT EXISTS %1 CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci").arg(m_dbName);
        if (!q.exec(createdb)) {
            qDebug() << "Error creating database:" << q.lastError().text();
            tmp.close();
            QSqlDatabase::removeDatabase(tempConnName);
            return false;
        }

        tmp.close();
        QSqlDatabase::removeDatabase(tempConnName);

        // Try opening again now that DB exists
        m_database = QSqlDatabase::addDatabase("QMYSQL");
        m_database.setHostName(m_host);
        m_database.setPort(m_port);
        m_database.setUserName(m_user);
        m_database.setPassword(m_password);
        m_database.setDatabaseName(m_dbName);

        if (!m_database.open()) {
            qDebug() << "Error: connection with database failed after creation -" << m_database.lastError().text();
            return false;
        }
    }

    qDebug() << "Database: MySQL connection ok, host:" << m_host << "db:" << m_dbName;

    if (!createTables()) {
        qDebug() << "Error: failed to create tables";
        return false;
    }

    if (!insertDefaultData()) {
        qDebug() << "Error: failed to insert default data";
        return false;
    }

    return true;
}

QSqlDatabase& DBConnection::database()
{
    return m_database;
}

bool DBConnection::createTables()
{
    QSqlQuery query(m_database);

    // Drop old tables to ensure fresh schema (order matters due to FKs)
    QStringList tablesToDrop = {"enrollments", "payments", "students_data", "sections", "courses", "semester", "news", "users"};
    for (const QString& table : tablesToDrop) {
        query.exec("DROP TABLE IF EXISTS `" + table + "`");
    }

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

    // Semester table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `semester` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "year DATETIME,"
                    "semester INT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating semester table:" << query.lastError().text();
        return false;
    }

    // Courses table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `courses` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(255) NOT NULL,"
                    "description TEXT,"
                    "year_level INT,"
                    "credit_hours INT,"
                    "semester_id INT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL,"
                    "FOREIGN KEY (semester_id) REFERENCES semester(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating courses table:" << query.lastError().text();
        return false;
    }

    // Sections table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `sections` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "course_id INT,"
                    "capacity INT DEFAULT 40,"
                    "semester_id INT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL,"
                    "FOREIGN KEY (course_id) REFERENCES courses(id),"
                    "FOREIGN KEY (semester_id) REFERENCES semester(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating sections table:" << query.lastError().text();
        return false;
    }

    // Students Data table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `students_data` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "user_id INT,"
                    "student_number VARCHAR(100) UNIQUE NOT NULL,"
                    "id_number VARCHAR(100) NOT NULL,"
                    "dob DATETIME NULL,"
                    "year INT NULL,"
                    "department VARCHAR(255) NULL,"
                    "section_id INT NULL,"
                    "seat_number VARCHAR(50) NULL,"
                    "status VARCHAR(50) DEFAULT 'active',"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL,"
                    "FOREIGN KEY (user_id) REFERENCES users(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating students_data table:" << query.lastError().text();
        return false;
    }

    // Enrollments table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `enrollments` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "student_id INT,"
                    "status VARCHAR(50) DEFAULT 'active',"
                    "assessments_grades TEXT,"
                    "Assignment_1 TEXT,"
                    "Assignment_2 TEXT,"
                    "final TEXT,"
                    "total_marks INT,"
                    "is_rafaa_applied TINYINT(1),"
                    "enrolled_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "created_at DATETIME NULL,"
                    "FOREIGN KEY (student_id) REFERENCES students_data(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating enrollments table:" << query.lastError().text();
        return false;
    }

    // Payments table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `payments` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "student_id INT,"
                    "amount DOUBLE,"
                    "date DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "year DATETIME,"
                    "method VARCHAR(100),"
                    "status VARCHAR(50) DEFAULT 'unpaid',"
                    "notes TEXT,"
                    "created_at DATETIME NULL,"
                    "FOREIGN KEY (student_id) REFERENCES students_data(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating payments table:" << query.lastError().text();
        return false;
    }

    // News table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `news` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "title VARCHAR(255),"
                    "body TEXT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating news table:" << query.lastError().text();
        return false;
    }

    qDebug() << "All tables created successfully (New Schema)";
    return true;
}

bool DBConnection::insertDefaultData()
{
    QSqlQuery query(m_database);

    // Check if admin already exists
    query.exec("SELECT COUNT(*) FROM users WHERE role = 'Admin'");
    if (query.next() && query.value(0).toInt() > 0) {
        qDebug() << "Default data already exists";
        return true;
    }

    // Insert default admin (username: admin, password: admin123)
    QString password = "admin123";
    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    query.prepare("INSERT INTO users (full_name, username, password, role) VALUES (?, ?, ?, ?)");
    query.addBindValue("System Administrator");
    query.addBindValue("admin");
    query.addBindValue(passwordHash);
    query.addBindValue("Admin");

    if (!query.exec()) {
        qDebug() << "Error inserting default admin:" << query.lastError().text();
        return false;
    }

    qDebug() << "Default admin created (username: admin, password: admin123)";
    return true;
}
