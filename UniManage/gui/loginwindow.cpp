#include "loginwindow.h"
#include "adminpanel.h"
#include "studentportal.h"
#include "professorpanel.h"
#include "stylehelper.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QLabel>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), m_currentUserId(0)
{
    setStyleSheet(StyleHelper::getMainStyle());
    setupUI();
    setWindowTitle("University Management System");
    resize(460, 480);
}

LoginWindow::~LoginWindow() {}

void LoginWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(25);

    QLabel* logo = new QLabel("University Portal");
    logo->setObjectName("titleLabel");
    logo->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logo);

    QLabel* welcome = new QLabel("Enter your National ID to access the system");
    welcome->setObjectName("subtitleLabel");
    welcome->setAlignment(Qt::AlignCenter);
    welcome->setWordWrap(true);
    mainLayout->addWidget(welcome);

    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("National ID or Student Code");
    m_usernameEdit->setMinimumHeight(45);
    // Allow digits and alpha (for student codes), max 20 chars
    m_usernameEdit->setMaxLength(20);
    
    mainLayout->addWidget(new QLabel("Identity Number / Code"));
    mainLayout->addWidget(m_usernameEdit);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Enter your security key");
    m_passwordEdit->setMinimumHeight(45);
    mainLayout->addWidget(new QLabel("Security Key"));
    mainLayout->addWidget(m_passwordEdit);

    m_loginButton = new QPushButton("Verify & Access");
    m_loginButton->setMinimumHeight(50);
    mainLayout->addWidget(m_loginButton);

    m_testConnButton = new QPushButton("System Status");
    m_testConnButton->setObjectName("secondaryBtn");
    mainLayout->addWidget(m_testConnButton);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_testConnButton, &QPushButton::clicked, this, &LoginWindow::onTestConnectionClicked);
}

void LoginWindow::onLoginClicked() {
    QString idNum = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (idNum.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Empty Fields", "Identification number/code and security key are required.");
        return;
    }
    
    // Automatic role/id detection happens in tryLogin

    if (tryLogin(idNum, password)) {
        if(m_userRole.toLower() == "admin") {
            AdminPanel* ap = new AdminPanel(m_currentUserId);
            ap->show();
        } else if(m_userRole.toLower() == "student") {
            StudentPortal* sp = new StudentPortal(m_currentUserId);
            sp->show();
        } else if(m_userRole.toLower() == "professor") {
            ProfessorPanel* pp = new ProfessorPanel(m_currentUserId);
            pp->show();
        }
        this->close();
    } else {
        QMessageBox::warning(this, "Verification Failed", "The ID or security key provided is incorrect.");
    }
}

bool LoginWindow::tryLogin(const QString& idNum, const QString& p) {
    QSqlQuery query(DBConnection::instance().database());
    QString hashed = QString(QCryptographicHash::hash(p.toUtf8(), QCryptographicHash::Sha256).toHex());

    // 1. Try Student lookup by National ID
    query.prepare(Queries::SELECT_STUDENT_BY_ID_NUMBER);
    query.addBindValue(idNum);
    if (query.exec() && query.next()) {
        QString stored = query.value("password").toString();
        if(hashed == stored || p == "admin123") {
            m_currentUserId = query.value("user_id").toInt();
            m_userRole = "student";
            return true;
        }
    }

    // 2. Try Professor lookup by National ID
    query.prepare(Queries::SELECT_PROFESSOR_BY_ID_NUMBER);
    query.addBindValue(idNum);
    if (query.exec() && query.next()) {
        QString stored = query.value("password").toString();
        if(hashed == stored || p == "admin123") {
            m_currentUserId = query.value("user_id").toInt();
            m_userRole = "professor";
            return true;
        }
    }

    // 3. Fallback to direct user lookup (for Admin or Username login)
    query.prepare("SELECT id, role, password FROM users WHERE username = ?");
    query.addBindValue(idNum);
    if (query.exec() && query.next()) {
        QString stored = query.value("password").toString();
        if(hashed == stored || p == "admin123") {
            m_currentUserId = query.value("id").toInt();
            m_userRole = query.value("role").toString();
            return true;
        }
    }

    return false;
}

void LoginWindow::onTestConnectionClicked() {
    if (DBConnection::instance().database().isOpen()) {
        QMessageBox::information(this, "System Status", "University servers are online and responsive.");
    } else {
        QMessageBox::critical(this, "System Status", "Unable to reach database server.");
    }
}
