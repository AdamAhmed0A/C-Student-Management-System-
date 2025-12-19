#include "dbconnection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QSqlDatabase>

DBConnection::DBConnection()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_databasePath = appDataPath + "/university.db";
    qDebug() << "Database file path:" << m_databasePath;
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
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open()) {
        qDebug() << "Error: connection with database failed -" << m_database.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok, path:" << m_databasePath;

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

    // Drop old tables to ensure fresh schema
    QStringList tablesToDrop = {"students", "admins", "grades", "forgiveness_policies",
                                "users", "students_data", "semester", "courses", "sections", "enrollments", "payments", "news"};
    for (const QString& table : tablesToDrop) {
        query.exec("DROP TABLE IF EXISTS " + table);
    }

    // Users table
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "full_name TEXT NOT NULL,"
                    "username TEXT UNIQUE NOT NULL,"
                    "password TEXT NOT NULL,"
                    "role TEXT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME)")) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // Semester table
    if (!query.exec("CREATE TABLE IF NOT EXISTS semester ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "year DATETIME,"
                    "semester INTEGER,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME)")) {
        qDebug() << "Error creating semester table:" << query.lastError().text();
        return false;
    }

    // Courses table
    if (!query.exec("CREATE TABLE IF NOT EXISTS courses ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL,"
                    "description TEXT,"
                    "year_level INTEGER,"
                    "credit_hours INTEGER,"
                    "semester_id INTEGER,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME,"
                    "FOREIGN KEY (semester_id) REFERENCES semester(id))")) {
        qDebug() << "Error creating courses table:" << query.lastError().text();
        return false;
    }

    // Sections table
    if (!query.exec("CREATE TABLE IF NOT EXISTS sections ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "course_id INTEGER,"
                    "capacity INTEGER DEFAULT 40,"
                    "semester_id INTEGER,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME,"
                    "FOREIGN KEY (course_id) REFERENCES courses(id),"
                    "FOREIGN KEY (semester_id) REFERENCES semester(id))")) {
        qDebug() << "Error creating sections table:" << query.lastError().text();
        return false;
    }

    // Students Data table
    if (!query.exec("CREATE TABLE IF NOT EXISTS students_data ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "user_id INTEGER,"
                    "student_number TEXT UNIQUE NOT NULL,"
                    "id_number TEXT NOT NULL,"
                    "dob DATETIME,"
                    "year INTEGER,"
                    "department TEXT,"
                    "section_id INTEGER,"
                    "seat_number TEXT,"
                    "status TEXT DEFAULT 'active',"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME,"
                    "FOREIGN KEY (user_id) REFERENCES users(id),"
                    "FOREIGN KEY (section_id) REFERENCES sections(id))")) {
        qDebug() << "Error creating students_data table:" << query.lastError().text();
        return false;
    }

    // Enrollments table
    if (!query.exec("CREATE TABLE IF NOT EXISTS enrollments ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "student_id INTEGER,"
                    "status TEXT DEFAULT 'active',"
                    "assessments_grades TEXT,"
                    "Assignment_1 TEXT,"
                    "Assignment_2 TEXT,"
                    "final TEXT,"
                    "total_marks INTEGER,"
                    "is_rafaa_applied INTEGER,"
                    "enrolled_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "created_at DATETIME,"
                    "FOREIGN KEY (student_id) REFERENCES students_data(id))")) {
        qDebug() << "Error creating enrollments table:" << query.lastError().text();
        return false;
    }

    // Payments table
    if (!query.exec("CREATE TABLE IF NOT EXISTS payments ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "student_id INTEGER,"
                    "amount REAL,"
                    "date DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "year DATETIME,"
                    "method TEXT,"
                    "status TEXT DEFAULT 'unpaid',"
                    "notes TEXT,"
                    "created_at DATETIME,"
                    "FOREIGN KEY (student_id) REFERENCES students_data(id))")) {
        qDebug() << "Error creating payments table:" << query.lastError().text();
        return false;
    }

    // News table
    if (!query.exec("CREATE TABLE IF NOT EXISTS news ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "title TEXT,"
                    "body TEXT,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at DATETIME)")) {
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
