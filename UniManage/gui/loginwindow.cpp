#include "loginwindow.h"
#include "adminpanel.h"
#include "studentportal.h"
#include "../database/dbconnection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), m_currentAdminId(0), m_currentStudentId(0)
{
    setupUI();
    setWindowTitle("University SIS - Login");
    resize(400, 300);
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    
    QLabel* titleLabel = new QLabel("University Student Information System", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QGroupBox* loginGroup = new QGroupBox("Login", this);
    QVBoxLayout* loginLayout = new QVBoxLayout(loginGroup);
    loginLayout->setSpacing(15);
    
    QLabel* userTypeLabel = new QLabel("Login as:", this);
    loginLayout->addWidget(userTypeLabel);
    
    m_userTypeCombo = new QComboBox(this);
    m_userTypeCombo->addItem("Administrator");
    m_userTypeCombo->addItem("Student");
    connect(m_userTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LoginWindow::onUserTypeChanged);
    loginLayout->addWidget(m_userTypeCombo);
    
    m_usernameLabel = new QLabel("Username:", this);
    loginLayout->addWidget(m_usernameLabel);
    
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Enter username");
    loginLayout->addWidget(m_usernameEdit);
    
    m_passwordLabel = new QLabel("Password:", this);
    loginLayout->addWidget(m_passwordLabel);
    
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Enter password");
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    loginLayout->addWidget(m_passwordEdit);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_testConnButton = new QPushButton("Test Connection", this);
    m_testConnButton->setMinimumHeight(35);
    connect(m_testConnButton, &QPushButton::clicked, this, &LoginWindow::onTestConnectionClicked);
    btnLayout->addWidget(m_testConnButton);

    m_loginButton = new QPushButton("Login", this);
    m_loginButton->setMinimumHeight(35);
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    btnLayout->addWidget(m_loginButton);

    loginLayout->addLayout(btnLayout);
    
    mainLayout->addWidget(loginGroup);
    mainLayout->addStretch();
    
    QLabel* infoLabel = new QLabel("Default Admin: username = admin, password = admin123", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
}

void LoginWindow::onUserTypeChanged(int index)
{
    if (index == 0) {
        m_usernameLabel->setText("Username:");
        m_usernameEdit->setPlaceholderText("Enter username");
        m_passwordLabel->setVisible(true);
        m_passwordEdit->setVisible(true);
    } else {
        m_usernameLabel->setText("Student Number:");
        m_usernameEdit->setPlaceholderText("Enter student number");
        m_passwordLabel->setVisible(false);
        m_passwordEdit->setVisible(false);
    }
}

void LoginWindow::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your credentials");
        return;
    }
    
    if (m_userTypeCombo->currentIndex() == 0) {
        if (password.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter password");
            return;
        }
        
        if (loginAdmin(username, password)) {
            AdminPanel* adminPanel = new AdminPanel(m_currentAdminId);
            adminPanel->show();
            this->close();
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid username or password");
        }
    } else {
        if (loginStudent(username)) {
            StudentPortal* studentPortal = new StudentPortal(m_currentStudentId);
            studentPortal->show();
            this->close();
        } else {
            QMessageBox::warning(this, "Login Failed", "Student number not found");
        }
    }
}

bool LoginWindow::loginAdmin(const QString& username, const QString& password)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("SELECT id, password FROM users WHERE username = ? AND role = 'Admin'");
    query.addBindValue(username);
    
    if (!query.exec()) {
        qDebug() << "Error checking admin:" << query.lastError().text();
        return false;
    }
    
    if (query.next()) {
        QString storedHash = query.value("password").toString();
        QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
        
        if (passwordHash == storedHash) {
            m_currentAdminId = query.value("id").toInt();
            return true;
        }
    }
    
    return false;
}

bool LoginWindow::loginStudent(const QString& studentNumber)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("SELECT id FROM students_data WHERE student_number = ?");
    query.addBindValue(studentNumber);
    
    if (!query.exec()) {
        qDebug() << "Error checking student:" << query.lastError().text();
        return false;
    }
    
    if (query.next()) {
        m_currentStudentId = query.value("id").toInt();
        return true;
    }
    
    return false;
}

void LoginWindow::onTestConnectionClicked()
{
    // If DB is open, run a simple query to validate full connectivity
    QSqlDatabase& db = DBConnection::instance().database();
    if (db.isOpen()) {
        QSqlQuery test(db);
        if (test.exec("SELECT 1")) {
            QMessageBox::information(this, "Connection Test", "MySQL connection is open and query executed successfully.");
        } else {
            QMessageBox::warning(this, "Connection Test", QString("Connected but test query failed: %1").arg(test.lastError().text()));
        }
        return;
    }

    // Try to initialize (will attempt to create DB if missing)
    if (DBConnection::instance().initialize()) {
        QMessageBox::information(this, "Connection Test", "Connection established and database initialized.");
    } else {
        QMessageBox::critical(this, "Connection Test", "Failed to initialize database connection. Check application logs for details.");
    }
}
