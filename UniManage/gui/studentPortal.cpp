#include "studentportal.h"
#include "stylehelper.h"
#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>

StudentPortal::StudentPortal(int studentUserId, QWidget *parent)
    : QWidget(parent), m_userId(studentUserId)
{
    setStyleSheet(StyleHelper::getMainStyle());
    loadStudentData();
    setupUI();
    setWindowTitle("Student Portal - University Management System");
    resize(1000, 700);
}

StudentPortal::~StudentPortal() {}

void StudentPortal::loadStudentData() {
    m_student = m_studentController.getStudentByUserId(m_userId);
}

void StudentPortal::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    QHBoxLayout* header = new QHBoxLayout();
    m_nameLabel = new QLabel("Welcome, " + m_student.fullName());
    m_nameLabel->setObjectName("titleLabel");
    header->addWidget(m_nameLabel);
    header->addStretch();
    
    m_gpaLabel = new QLabel("Current GPA: 0.00");
    m_gpaLabel->setObjectName("subtitleLabel");
    m_gpaLabel->setStyleSheet("color: #059669; font-weight: bold; background: #ecfdf5; padding: 10px; border-radius: 8px;");
    header->addWidget(m_gpaLabel);
    
    QPushButton* refreshBtn = new QPushButton("Refresh");
    refreshBtn->setObjectName("secondaryBtn");
    refreshBtn->setFixedWidth(100);
    connect(refreshBtn, &QPushButton::clicked, this, &StudentPortal::onRefreshAll);
    header->addWidget(refreshBtn);

    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    logoutBtn->setFixedWidth(100);
    connect(logoutBtn, &QPushButton::clicked, this, &StudentPortal::onLogout);
    header->addWidget(logoutBtn);
    
    mainLayout->addLayout(header);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createDashboardTab(), "Academic Summary");
    m_tabWidget->addTab(createGradesTab(), "My Grades");
    m_tabWidget->addTab(createScheduleTab(), "Weekly Schedule");
    m_tabWidget->addTab(createPaymentsTab(), "Tuition Fees");

    mainLayout->addWidget(m_tabWidget);
}

void StudentPortal::onLogout() {
    LoginWindow* login = new LoginWindow();
    login->show();
    this->close();
}

void StudentPortal::onRefreshAll() {
    loadStudentData();
    refreshGrades();
    refreshPayments();
    QMessageBox::information(this, "Refreshed", "Your student profile and academic records have been updated.");
}

QWidget* StudentPortal::createDashboardTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QGroupBox* profile = new QGroupBox("Student Registration Details");
    QFormLayout* fl = new QFormLayout(profile);
    fl->addRow("Student Code:", new QLabel(m_student.studentNumber()));
    fl->addRow("National ID:", new QLabel(m_student.idNumber()));
    fl->addRow("Faculty/Dept:", new QLabel(m_student.department()));
    fl->addRow("Academic Level:", new QLabel(QString::number(m_student.academicLevelId())));
    fl->addRow("Section Number:", new QLabel(QString::number(m_student.sectionId())));
    
    layout->addWidget(profile);
    layout->addStretch();
    return widget;
}

QWidget* StudentPortal::createGradesTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    m_gradesTable = new QTableWidget();
    m_gradesTable->setColumnCount(7);
    m_gradesTable->setHorizontalHeaderLabels({"Course", "Ass 1", "Ass 2", "CW", "Final", "Total", "Evaluation"});
    m_gradesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_gradesTable);
    
    refreshGrades();
    return widget;
}

QWidget* StudentPortal::createScheduleTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    m_scheduleTable = new QTableWidget();
    m_scheduleTable->setColumnCount(5);
    m_scheduleTable->setHorizontalHeaderLabels({"Day", "Course", "Room", "Start Time", "End Time"});
    m_scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_scheduleTable);
    return widget;
}

QWidget* StudentPortal::createPaymentsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    m_paymentsTable = new QTableWidget();
    m_paymentsTable->setColumnCount(5);
    m_paymentsTable->setHorizontalHeaderLabels({"Date", "Amount", "Method", "Status", "Notes"});
    m_paymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_paymentsTable);
    
    refreshPayments();
    return widget;
}

void StudentPortal::refreshGrades() {
    m_gradesTable->setRowCount(0);
    double totalPoints = 0;
    int courses = 0;
    
    for(const auto& e : m_enrollmentController.getEnrollmentsByStudent(m_student.id())) {
        int r = m_gradesTable->rowCount();
        m_gradesTable->insertRow(r);
        m_gradesTable->setItem(r, 0, new QTableWidgetItem(e.courseName()));
        m_gradesTable->setItem(r, 1, new QTableWidgetItem(QString::number(e.assignment1Grade())));
        m_gradesTable->setItem(r, 2, new QTableWidgetItem(QString::number(e.assignment2Grade())));
        m_gradesTable->setItem(r, 3, new QTableWidgetItem(QString::number(e.courseworkGrade())));
        m_gradesTable->setItem(r, 4, new QTableWidgetItem(QString::number(e.finalExamGrade())));
        m_gradesTable->setItem(r, 5, new QTableWidgetItem(QString::number(e.totalGrade())));
        m_gradesTable->setItem(r, 6, new QTableWidgetItem(e.letterGrade()));
        
        if(e.letterGrade() == "Excellent") totalPoints += 4;
        else if(e.letterGrade() == "Very Good") totalPoints += 3;
        else if(e.letterGrade() == "Good") totalPoints += 2;
        else if(e.letterGrade() == "Pass") totalPoints += 1;
        courses++;
    }
    
    if(courses > 0) {
        m_gpaLabel->setText(QString("Current GPA: %1").arg(totalPoints/courses, 0, 'f', 2));
    }
}

void StudentPortal::refreshPayments() {
    m_paymentsTable->setRowCount(0);
    for(const auto& p : m_paymentController.getPaymentsByStudent(m_student.id())) {
        int r = m_paymentsTable->rowCount();
        m_paymentsTable->insertRow(r);
        m_paymentsTable->setItem(r, 0, new QTableWidgetItem(p.date().toString("yyyy-MM-dd")));
        m_paymentsTable->setItem(r, 1, new QTableWidgetItem(QString::number(p.amount())));
        m_paymentsTable->setItem(r, 2, new QTableWidgetItem(p.method()));
        m_paymentsTable->setItem(r, 3, new QTableWidgetItem(p.status()));
        m_paymentsTable->setItem(r, 4, new QTableWidgetItem(p.notes()));
    }
}

void StudentPortal::calculateGPA() {}
