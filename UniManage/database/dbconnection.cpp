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

    qDebug() << "=== ATTEMPTING DATABASE CONNECTION ===";
    qDebug() << "Host:" << m_host;
    qDebug() << "Port:" << m_port;
    qDebug() << "User:" << m_user;
    qDebug() << "Database:" << m_dbName;
    qDebug() << "Password:" << m_password;

    if (!m_database.open()) {
        m_lastError = QString("Connection Failed.\nError: %1\nDriver Error: %2\nDatabase Error: %3")
                          .arg(m_database.lastError().text())
                          .arg(m_database.lastError().driverText())
                          .arg(m_database.lastError().databaseText());
                          
        qDebug() << "=== DATABASE CONNECTION FAILED ===";
        qDebug() << "Error:" << m_database.lastError().text();
        qDebug() << "Error Type:" << m_database.lastError().type();
        qDebug() << "Driver Error:" << m_database.lastError().driverText();
        qDebug() << "Database Error:" << m_database.lastError().databaseText();

        // Try to create the database if it doesn't exist by connecting without a DB
        qDebug() << "Attempting to create database...";
        QString tempConnName = "__create_db_conn__";
        QSqlDatabase tmp = QSqlDatabase::addDatabase("QMYSQL", tempConnName);
        tmp.setHostName(m_host);
        tmp.setPort(m_port);
        tmp.setUserName(m_user);
        tmp.setPassword(m_password);

        if (!tmp.open()) {
            m_lastError = QString("Cannot connect to MySQL Server.\nError: %1\n\nIs MySQL running? Check username/password.")
                              .arg(tmp.lastError().text());
                              
            qDebug() << "=== CANNOT CONNECT TO MYSQL SERVER ===";
            qDebug() << "Error:" << tmp.lastError().text();
            qDebug() << "";
            qDebug() << "TROUBLESHOOTING:";
            qDebug() << "1. Check if MySQL is running";
            qDebug() << "2. Verify username and password";
            qDebug() << "3. Check if MySQL is listening on" << m_host << ":" << m_port;
            qDebug() << "";
            qDebug() << "To set custom credentials, use environment variables:";
            qDebug() << "  DB_HOST=127.0.0.1";
            qDebug() << "  DB_PORT=3306";
            qDebug() << "  DB_USER=root";
            qDebug() << "  DB_PASSWORD=your_password";
            qDebug() << "  DB_NAME=university";
            QSqlDatabase::removeDatabase(tempConnName);
            return false;
        }

        QSqlQuery q(tmp);
        QString createdb = QString("CREATE DATABASE IF NOT EXISTS %1 CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci").arg(m_dbName);
        if (!q.exec(createdb)) {
            m_lastError = "Failed to create database: " + q.lastError().text();
            qDebug() << "Error creating database:" << q.lastError().text();
            tmp.close();
            QSqlDatabase::removeDatabase(tempConnName);
            return false;
        }

        qDebug() << "Database created successfully!";
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
            m_lastError = "Connection failed after creating DB: " + m_database.lastError().text();
            qDebug() << "=== ERROR: CONNECTION FAILED AFTER DATABASE CREATION ===";
            qDebug() << "Error:" << m_database.lastError().text();
            return false;
        }
    }

    qDebug() << "=== DATABASE CONNECTION SUCCESSFUL ===";
    qDebug() << "MySQL connection ok, host:" << m_host << "db:" << m_dbName;

    if (!createTables()) {
        m_lastError = "Failed to create tables.";
        qDebug() << "Error: failed to create tables";
        return false;
    }
    
    // Only insert default data if users table is empty
    QSqlQuery checkUsers(m_database);
    if (checkUsers.exec("SELECT COUNT(*) FROM users") && checkUsers.next()) {
        if (checkUsers.value(0).toInt() == 0) {
            insertDefaultData();
        }
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

    // Migration: Ensure role column exists (for older database users)
    query.exec("ALTER TABLE `users` ADD COLUMN IF NOT EXISTS `role` VARCHAR(50) AFTER `password` ");

    // Colleges (Faculty) table - MOVED UP
    if (!query.exec("CREATE TABLE IF NOT EXISTS `colleges` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(255) NOT NULL,"
                    "code VARCHAR(50) UNIQUE) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating colleges table:" << query.lastError().text();
        return false;
    }

    // Departments table - MOVED UP
    if (!query.exec("CREATE TABLE IF NOT EXISTS `departments` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(255) NOT NULL,"
                    "college_id INT,"
                    "code VARCHAR(50) UNIQUE,"
                    "FOREIGN KEY (college_id) REFERENCES colleges(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating departments table:" << query.lastError().text();
        return false;
    }

    // Academic Levels table - MOVED UP
    if (!query.exec("CREATE TABLE IF NOT EXISTS `academic_levels` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(100) NOT NULL,"
                    "level_number INT) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating academic_levels table:" << query.lastError().text();
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

    // Courses table (expanded with Name, Description, Degrees, Type)
    if (!query.exec("CREATE TABLE IF NOT EXISTS `courses` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(255) NOT NULL,"
                    "description TEXT,"
                    "year_level INT,"
                    "credit_hours INT,"
                    "semester_id INT,"
                    "max_grade INT DEFAULT 100," // 100 or 150
                    "course_type VARCHAR(50) DEFAULT 'Theoretical'," // 'Theoretical' or 'Practical'
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
                    "department VARCHAR(255) NULL,"
                    "department_id INT NULL,"
                    "academic_level_id INT NULL,"
                    "section_id INT NULL,"
                    "seat_number VARCHAR(50) NULL,"
                    "status VARCHAR(50) DEFAULT 'active',"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL,"
                    "FOREIGN KEY (user_id) REFERENCES users(id),"
                     "FOREIGN KEY (department_id) REFERENCES departments(id)," // Now valid
                     "FOREIGN KEY (academic_level_id) REFERENCES academic_levels(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) { // Now valid
        qDebug() << "Error creating students_data table:" << query.lastError().text();
        return false;
    }

    // Migration: Ensure id_number exists in students_data
    query.exec("ALTER TABLE `students_data` ADD COLUMN IF NOT EXISTS `id_number` VARCHAR(100) AFTER `student_number` ");
    query.exec("ALTER TABLE `students_data` ADD COLUMN IF NOT EXISTS `department_id` INT NULL AFTER `department` ");
    query.exec("ALTER TABLE `students_data` ADD COLUMN IF NOT EXISTS `academic_level_id` INT NULL AFTER `department_id` ");

    // Enrollments table (expanded with Grade Distribution)
    if (!query.exec("CREATE TABLE IF NOT EXISTS `enrollments` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "student_id INT,"
                    "course_id INT,"
                    "status VARCHAR(50) DEFAULT 'active',"
                    "attendance_count INT DEFAULT 0,"
                    "absence_count INT DEFAULT 0,"
                    "assignment_1_grade DOUBLE DEFAULT 0,"
                    "assignment_2_grade DOUBLE DEFAULT 0,"
                    "coursework_grade DOUBLE DEFAULT 0,"
                    "final_exam_grade DOUBLE DEFAULT 0,"
                    "total_grade DOUBLE DEFAULT 0,"
                    "letter_grade VARCHAR(10)," // Excellent, Very Good, etc.
                    "is_rafaa_applied TINYINT(1) DEFAULT 0,"
                    "enrolled_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "FOREIGN KEY (student_id) REFERENCES students_data(id),"
                    "FOREIGN KEY (course_id) REFERENCES courses(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
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
                    "FOREIGN KEY (student_id) REFERENCES students_data(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating payments table:" << query.lastError().text();
        return false;
    }
    
    // Rooms (Halls & Labs) table expanded
    if (!query.exec("CREATE TABLE IF NOT EXISTS `rooms` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "name VARCHAR(100) NOT NULL,"
                    "type VARCHAR(50)," // 'Hall' or 'Lab'
                    "capacity INT,"
                    "ac_units INT DEFAULT 0,"
                    "fans_count INT DEFAULT 0,"
                    "lighting_points INT DEFAULT 0,"
                    "computers_count INT DEFAULT 0," // For Labs
                    "seating_description TEXT,"
                    "code VARCHAR(50) UNIQUE) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating rooms table:" << query.lastError().text();
        return false;
    }

    // Detailed List Specification Table for Hall/Lab Equipment
    if (!query.exec("CREATE TABLE IF NOT EXISTS `room_specs` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "room_id INT,"
                    "product_id VARCHAR(100),"
                    "product_name VARCHAR(255),"
                    "product_description TEXT,"
                    "FOREIGN KEY (room_id) REFERENCES rooms(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating room_specs table:" << query.lastError().text();
        return false;
    }
    
    // Professors (Doctors) table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `professors` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "user_id INT,"
                    "id_number VARCHAR(100) UNIQUE NULL,"
                    "specialization VARCHAR(255),"
                    "title VARCHAR(100),"
                    "personal_info TEXT,"
                    "FOREIGN KEY (user_id) REFERENCES users(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating professors table:" << query.lastError().text();
        return false;
    }

    // Migration: Ensure id_number exists in professors
    query.exec("ALTER TABLE `professors` ADD COLUMN IF NOT EXISTS `id_number` VARCHAR(100) UNIQUE AFTER `user_id` ");

    // Academic Schedules table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `schedules` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "course_id INT,"
                    "room_id INT,"
                    "professor_id INT,"
                    "day_of_week VARCHAR(20),"
                    "start_time TIME,"
                    "end_time TIME,"
                    "FOREIGN KEY (course_id) REFERENCES courses(id),"
                    "FOREIGN KEY (room_id) REFERENCES rooms(id),"
                    "FOREIGN KEY (professor_id) REFERENCES professors(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating schedules table:" << query.lastError().text();
        return false;
    }

    // News table (Academic Calendar/Updates)
    if (!query.exec("CREATE TABLE IF NOT EXISTS `news` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "title VARCHAR(255),"
                    "body TEXT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating news table:" << query.lastError().text();
        return false;
    }

    // Attendance Logs table (New)
    if (!query.exec("CREATE TABLE IF NOT EXISTS `attendance_logs` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "enrollment_id INT,"
                    "date DATE,"
                    "status VARCHAR(50)," // Present, Absent, Late, Excused
                    "notes TEXT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "FOREIGN KEY (enrollment_id) REFERENCES enrollments(id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating attendance_logs table:" << query.lastError().text();
        return false;
    }

    // Calendar Events table
    if (!query.exec("CREATE TABLE IF NOT EXISTS `calendar_events` ("
                    "id INT PRIMARY KEY AUTO_INCREMENT,"
                    "title VARCHAR(255) NOT NULL,"
                    "description TEXT,"
                    "start_date DATE,"
                    "end_date DATE,"
                    "event_type VARCHAR(50)," // Holiday, Exam, Term, Deadline
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4")) {
        qDebug() << "Error creating calendar_events table:" << query.lastError().text();
        return false;
    }

    qDebug() << "All tables created/updated successfully for expanded SIS";
    return true;
}

bool DBConnection::insertDefaultData()
{
    QSqlQuery query(m_database);

    // Ensure default admin exists (username: admin, password: admin123)
    QString password = "admin123";
    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    query.prepare("INSERT IGNORE INTO users (full_name, username, password, role) VALUES (?, ?, ?, ?)");
    query.addBindValue("System Administrator");
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

QString DBConnection::getLastError() const
{
    return m_lastError;
}
