#include "studentPortal.h"
#include "stylehelper.h"
#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDialog>

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
    m_tabWidget->addTab(createCalendarTab(), "Calendar");

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
    refreshSchedule();
    refreshPayments();
    refreshCalendar();
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
    
    // Display Year/Level with Enrollment Year
    QString lvlStr = "Not Assigned";
    if (m_student.academicLevelId() > 0) {
        AcademicLevel al = m_academicLevelController.getAcademicLevelById(m_student.academicLevelId());
        if (al.id() > 0) {
            QString enrollYear = m_student.createdAt().isValid() ? 
                                QString::number(m_student.createdAt().date().year()) : "";
            
            if (!enrollYear.isEmpty()) {
                lvlStr = QString("Year %1 (%2)").arg(al.levelNumber()).arg(enrollYear);
            } else {
                lvlStr = QString("Year %1").arg(al.levelNumber());
            }
        }
    }
    fl->addRow("Year/Level:", new QLabel(lvlStr));
    
    QString sect = m_student.sectionName().isEmpty() ? (m_student.sectionId() == 0 ? "Not Assigned" : QString::number(m_student.sectionId())) : m_student.sectionName();
    fl->addRow("Assigned Group/Section:", new QLabel(sect));
    
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
    
    refreshSchedule();
    return widget;
}

QWidget* StudentPortal::createPaymentsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    m_paymentsTable = new QTableWidget();
    m_paymentsTable->setColumnCount(5);
    m_paymentsTable->setHorizontalHeaderLabels({"Date", "Amount", "Method", "Status", "Notes"});
    m_paymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    QGroupBox* summary = new QGroupBox("Financial Summary");
    QFormLayout* sl = new QFormLayout(summary);
    
    m_tuitionLabel = new QLabel("0.00");
    m_paidLabel = new QLabel("0.00");
    m_balanceLabel = new QLabel("0.00");
    m_balanceLabel->setStyleSheet("font-weight: bold; color: #b91c1c;"); // red
    
    sl->addRow("Faculty Base Tuition:", m_tuitionLabel);
    sl->addRow("Total Amount Paid:", m_paidLabel);
    sl->addRow("Remaining Balance:", m_balanceLabel);
    
    QPushButton* payBtn = new QPushButton("Pay Tuition Fees");
    payBtn->setObjectName("primaryBtn");
    sl->addRow(payBtn);
    connect(payBtn, &QPushButton::clicked, this, &StudentPortal::onMakePayment);
    
    layout->addWidget(summary);
    layout->addWidget(m_paymentsTable);
    
    refreshPayments();
    return widget;
}

QWidget* StudentPortal::createCalendarTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    m_calendarTable = new QTableWidget();
    m_calendarTable->setColumnCount(5);
    m_calendarTable->setHorizontalHeaderLabels({"Title", "Start Date", "End Date", "Type", "Description"});
    m_calendarTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_calendarTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_calendarTable);
    
    refreshCalendar();
    return widget;
}

void StudentPortal::refreshGrades() {
    m_gradesTable->setRowCount(0);
    double totalPoints = 0;
    int courses = 0;
    
    for(const auto& e : m_enrollmentController.getEnrollmentsByStudent(m_student.id())) {
        // Skip if all grades are zero (unassigned)
        if (e.assignment1Grade() == 0 && e.assignment2Grade() == 0 && 
            e.courseworkGrade() == 0 && e.finalExamGrade() == 0 && 
            e.experienceGrade() == 0) {
            continue;
        }

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
    auto payments = m_paymentController.getPaymentsByStudent(m_student.id());
    
    for(const auto& p : payments) {
        int r = m_paymentsTable->rowCount();
        m_paymentsTable->insertRow(r);
        m_paymentsTable->setItem(r, 0, new QTableWidgetItem(p.date().toString("yyyy-MM-dd")));
        m_paymentsTable->setItem(r, 1, new QTableWidgetItem(QString::number(p.amount())));
        m_paymentsTable->setItem(r, 2, new QTableWidgetItem(p.method()));
        m_paymentsTable->setItem(r, 3, new QTableWidgetItem(p.status()));
        m_paymentsTable->setItem(r, 4, new QTableWidgetItem(p.notes()));
    }
    
    // Update summary labels
    double baseTuition = 0.0;
    if (m_student.collegeId() > 0) {
        College c = m_collegeController.getCollegeById(m_student.collegeId());
        baseTuition = c.tuitionFees();
    }
    
    double totalPaid = 0.0;
    for (const auto& p : payments) {
        // Only count paid or completed payments
        if (p.status().compare("Paid", Qt::CaseInsensitive) == 0 || 
            p.status().compare("Success", Qt::CaseInsensitive) == 0 || 
            p.status().compare("Completed", Qt::CaseInsensitive) == 0) {
            totalPaid += p.amount();
        }
    }
    
    m_tuitionLabel->setText(QString::number(baseTuition, 'f', 2));
    m_paidLabel->setText(QString::number(totalPaid, 'f', 2));
    m_balanceLabel->setText(QString::number(baseTuition - totalPaid, 'f', 2));
}

void StudentPortal::refreshSchedule() {
    m_scheduleTable->setRowCount(0);
    
    // Strategy: Get all courses the student is enrolled in, then fetch their schedules
    auto enrollments = m_enrollmentController.getEnrollmentsByStudent(m_student.id());
    
    for (const auto& e : enrollments) {
        auto schedules = m_scheduleController.getScheduleByCourse(e.courseId());
        for (const auto& s : schedules) {
            int r = m_scheduleTable->rowCount();
            m_scheduleTable->insertRow(r);
            
            m_scheduleTable->setItem(r, 0, new QTableWidgetItem(s.dayOfWeek()));
            m_scheduleTable->setItem(r, 1, new QTableWidgetItem(s.courseName()));
            m_scheduleTable->setItem(r, 2, new QTableWidgetItem(s.roomName()));
            m_scheduleTable->setItem(r, 3, new QTableWidgetItem(s.startTime().toString("HH:mm")));
            m_scheduleTable->setItem(r, 4, new QTableWidgetItem(s.endTime().toString("HH:mm")));
        }
    }
    
    // Also try fetching by Level as fallback if no course-specific schedules found
    if (m_scheduleTable->rowCount() == 0 && m_student.academicLevelId() > 0) {
        auto levelSchedules = m_scheduleController.getScheduleByLevel(m_student.academicLevelId());
        for (const auto& s : levelSchedules) {
            int r = m_scheduleTable->rowCount();
            m_scheduleTable->insertRow(r);
            m_scheduleTable->setItem(r, 0, new QTableWidgetItem(s.dayOfWeek()));
            m_scheduleTable->setItem(r, 1, new QTableWidgetItem(s.courseName()));
            m_scheduleTable->setItem(r, 2, new QTableWidgetItem(s.roomName()));
            m_scheduleTable->setItem(r, 3, new QTableWidgetItem(s.startTime().toString("HH:mm")));
            m_scheduleTable->setItem(r, 4, new QTableWidgetItem(s.endTime().toString("HH:mm")));
        }
    }
    
    m_scheduleTable->sortByColumn(0, Qt::AscendingOrder); // Initial sort
}

void StudentPortal::onMakePayment() {
    if (m_student.id() == 0) {
        QMessageBox::warning(this, "Empty Profile", "You cannot make payments until your student profile is setup by an admin.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Process Tuition Payment");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QDoubleSpinBox* amount = new QDoubleSpinBox();
    amount->setRange(1.0, 100000.0);
    amount->setValue(0.0);
    
    QComboBox* method = new QComboBox();
    method->addItems({"Credit Card", "Bank Transfer", "Cash at Office", "Student App Balance"});
    
    QLineEdit* notes = new QLineEdit();
    notes->setPlaceholderText("Optional notes (e.g. Transaction ID)");
    
    layout->addRow("Payment Amount:", amount);
    layout->addRow("Payment Method:", method);
    layout->addRow("Notes:", notes);
    
    QPushButton* btn = new QPushButton("Confirm Payment");
    btn->setObjectName("primaryBtn");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    
    if (dialog.exec() == QDialog::Accepted) {
        Payment p;
        p.setStudentId(m_student.id());
        p.setAmount((float)amount->value());
        p.setMethod(method->currentText());
        p.setStatus("Paid"); // Automatically set to Paid for simulation
        p.setNotes(notes->text());
        p.setDate(QDateTime::currentDateTime());
        p.setYear(QDateTime::currentDateTime());
        
        if (m_paymentController.addPayment(p)) {
            QMessageBox::information(this, "Payment Successful", 
                QString("Successfully processed payment of %1. Your records have been updated.")
                .arg(QString::number(amount->value(), 'f', 2)));
            refreshPayments();
        } else {
            QMessageBox::critical(this, "Transaction Failed", "Could not record the payment in the database. Please contact the finance office.");
        }
    }
}

void StudentPortal::refreshCalendar() {
    m_calendarTable->setRowCount(0);
    for(const auto& e : m_calendarController.getAllEvents()) {
        int r = m_calendarTable->rowCount();
        m_calendarTable->insertRow(r);
        m_calendarTable->setItem(r, 0, new QTableWidgetItem(e.title()));
        m_calendarTable->setItem(r, 1, new QTableWidgetItem(e.startDate().toString("yyyy-MM-dd")));
        m_calendarTable->setItem(r, 2, new QTableWidgetItem(e.endDate().toString("yyyy-MM-dd")));
        m_calendarTable->setItem(r, 3, new QTableWidgetItem(e.eventType()));
        m_calendarTable->setItem(r, 4, new QTableWidgetItem(e.description()));
    }
}

void StudentPortal::calculateGPA() {}
