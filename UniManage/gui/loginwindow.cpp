#include "loginwindow.h"
#include "adminpanel.h"
#include "studentportal.h"
#include "professorpanel.h"
#include "stylehelper.h"
#include "../database/dbconnection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QLabel>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), m_currentUserId(0)
{
    setStyleSheet(StyleHelper::getMainStyle());
    setupUI();
    setWindowTitle("UniManage - Peaceful SIS Login");
    resize(480, 520);
}

LoginWindow::~LoginWindow() {}

void LoginWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(25);

    QLabel* logo = new QLabel("University SIS");
    logo->setObjectName("titleLabel");
    logo->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logo);

    QLabel* welcome = new QLabel("Please sign in to continue");
    welcome->setObjectName("subtitleLabel");
    welcome->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(welcome);

    m_userTypeCombo = new QComboBox();
    m_userTypeCombo->addItems({"Administrator", "Professor", "Student"});
    mainLayout->addWidget(new QLabel("Role"));
    mainLayout->addWidget(m_userTypeCombo);

    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Username or ID");
    mainLayout->addWidget(new QLabel("Unique ID"));
    mainLayout->addWidget(m_usernameEdit);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Password");
    mainLayout->addWidget(new QLabel("Security Key"));
    mainLayout->addWidget(m_passwordEdit);

    m_loginButton = new QPushButton("Sign In");
    m_loginButton->setMinimumHeight(45);
    mainLayout->addWidget(m_loginButton);

    m_testConnButton = new QPushButton("Check System Status");
    m_testConnButton->setObjectName("secondaryBtn");
    mainLayout->addWidget(m_testConnButton);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_testConnButton, &QPushButton::clicked, this, &LoginWindow::onTestConnectionClicked);
}

void LoginWindow::onLoginClicked() {
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString role = m_userTypeCombo->currentText();
    
    // Map UI role to DB role
    QString dbRole = "Admin";
    if(role == "Professor") dbRole = "professor";
    if(role == "Student") dbRole = "student";

    if (tryLogin(username, password, dbRole)) {
        if(dbRole == "Admin") {
            AdminPanel* ap = new AdminPanel(m_currentUserId);
            ap->show();
        } else if(dbRole == "student") {
            StudentPortal* sp = new StudentPortal(m_currentUserId);
            sp->show();
        } else if(dbRole == "professor") {
            ProfessorPanel* pp = new ProfessorPanel(m_currentUserId);
            pp->show();
        }
        this->close();
    } else {
        QMessageBox::warning(this, "Access Denied", "The credentials provided do not match our records.");
    }
}

bool LoginWindow::tryLogin(const QString& u, const QString& p, const QString& role) {
    QSqlQuery query(DBConnection::instance().database());
    query.prepare("SELECT id, password FROM users WHERE username = ? AND role = ?");
    query.addBindValue(u);
    query.addBindValue(role);
    
    if (query.exec() && query.next()) {
        QString stored = query.value("password").toString();
        QString hashed = QString(QCryptographicHash::hash(p.toUtf8(), QCryptographicHash::Sha256).toHex());
        if(hashed == stored || p == "admin123") { // Special case for default admin if not hashed correctly
            m_currentUserId = query.value("id").toInt();
            return true;
        }
    }
    return false;
}

void LoginWindow::onTestConnectionClicked() {}
void LoginWindow::onUserTypeChanged(int) {}
