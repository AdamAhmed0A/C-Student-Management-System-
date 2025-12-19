#include "professorpanel.h"
#include "stylehelper.h"
#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>

ProfessorPanel::ProfessorPanel(int userId, QWidget *parent)
    : QWidget(parent), m_userId(userId)
{
    setStyleSheet(StyleHelper::getMainStyle());
    loadProfessorData();
    setupUI();
    loadCourses();
    setWindowTitle("Instructor Portal - " + m_professor.fullName());
    resize(1100, 750);
}

ProfessorPanel::~ProfessorPanel() {}

void ProfessorPanel::loadProfessorData() {
    m_professor = m_professorController.getProfessorByUserId(m_userId);
}

void ProfessorPanel::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30,30,30,30);
    mainLayout->setSpacing(20);

    QHBoxLayout* header = new QHBoxLayout();
    QLabel* title = new QLabel("Instructor Dashboard");
    title->setObjectName("titleLabel");
    header->addWidget(title);
    header->addStretch();
    
    QPushButton* refreshBtn = new QPushButton("Refresh Data");
    refreshBtn->setObjectName("secondaryBtn");
    refreshBtn->setFixedWidth(130);
    connect(refreshBtn, &QPushButton::clicked, this, &ProfessorPanel::onRefreshAll);
    header->addWidget(refreshBtn);

    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    logoutBtn->setFixedWidth(120);
    connect(logoutBtn, &QPushButton::clicked, this, &ProfessorPanel::onLogout);
    header->addWidget(logoutBtn);
    
    mainLayout->addLayout(header);

    m_tabWidget = new QTabWidget();
    
    // Grading Tab
    QWidget* gradingTab = new QWidget();
    QVBoxLayout* gL = new QVBoxLayout(gradingTab);
    
    QHBoxLayout* ctrlH = new QHBoxLayout();
    ctrlH->addWidget(new QLabel("Select Course:"));
    m_courseSelector = new QComboBox();
    ctrlH->addWidget(m_courseSelector);
    ctrlH->addStretch();
    
    QPushButton* saveBtn = new QPushButton("Save All Grades");
    ctrlH->addWidget(saveBtn);
    gL->addLayout(ctrlH);

    m_studentsTable = new QTableWidget();
    m_studentsTable->setColumnCount(9);
    m_studentsTable->setHorizontalHeaderLabels({"EnrollID", "Student", "Att.", "Abs.", "Ass. 1", "Ass. 2", "CW", "Final", "Total"});
    m_studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gL->addWidget(m_studentsTable);
    
    m_tabWidget->addTab(gradingTab, "Grading & Attendance");

    // Schedule Tab
    QWidget* schedTab = new QWidget();
    QVBoxLayout* sL = new QVBoxLayout(schedTab);
    m_scheduleTable = new QTableWidget();
    m_scheduleTable->setColumnCount(4);
    m_scheduleTable->setHorizontalHeaderLabels({"Day", "Course", "Room", "Time"});
    sL->addWidget(m_scheduleTable);
    m_tabWidget->addTab(schedTab, "Academic Schedule");

    mainLayout->addWidget(m_tabWidget);

    connect(m_courseSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfessorPanel::onCourseSelected);
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onSaveGrades);
}

void ProfessorPanel::onLogout() {
    LoginWindow* login = new LoginWindow();
    login->show();
    this->close();
}

void ProfessorPanel::onRefreshAll() {
    loadCourses();
    onRefreshStudents();
    QMessageBox::information(this, "Refreshed", "Instructor data and student lists updated.");
}

void ProfessorPanel::loadCourses() {
    m_courseSelector->clear();
    for(const auto& c : m_courseController.getAllCourses()) {
        m_courseSelector->addItem(c.name(), c.id());
    }
}

void ProfessorPanel::onCourseSelected(int index) {
    if(index < 0) return;
    onRefreshStudents();
}

void ProfessorPanel::onRefreshStudents() {
    int courseId = m_courseSelector->currentData().toInt();
    m_studentsTable->setRowCount(0);
    
    for(const auto& e : m_enrollmentController.getEnrollmentsByCourse(courseId)) {
        int r = m_studentsTable->rowCount();
        m_studentsTable->insertRow(r);
        m_studentsTable->setItem(r, 0, new QTableWidgetItem(QString::number(e.id())));
        m_studentsTable->setItem(r, 1, new QTableWidgetItem(e.studentName()));
        
        QTableWidgetItem* att = new QTableWidgetItem(QString::number(e.attendanceCount()));
        QTableWidgetItem* abs = new QTableWidgetItem(QString::number(e.absenceCount()));
        QTableWidgetItem* a1 = new QTableWidgetItem(QString::number(e.assignment1Grade()));
        QTableWidgetItem* a2 = new QTableWidgetItem(QString::number(e.assignment2Grade()));
        QTableWidgetItem* cw = new QTableWidgetItem(QString::number(e.courseworkGrade()));
        QTableWidgetItem* fi = new QTableWidgetItem(QString::number(e.finalExamGrade()));
        
        m_studentsTable->setItem(r, 2, att);
        m_studentsTable->setItem(r, 3, abs);
        m_studentsTable->setItem(r, 4, a1);
        m_studentsTable->setItem(r, 5, a2);
        m_studentsTable->setItem(r, 6, cw);
        m_studentsTable->setItem(r, 7, fi);
        
        QTableWidgetItem* tot = new QTableWidgetItem(QString::number(e.totalGrade()));
        tot->setFlags(tot->flags() & ~Qt::ItemIsEditable);
        m_studentsTable->setItem(r, 8, tot);
    }
}

void ProfessorPanel::onSaveGrades() {
    int cid = m_courseSelector->currentData().toInt();
    Course course = m_courseController.getCourseById(cid);
    bool success = true;
    
    for(int i=0; i < m_studentsTable->rowCount(); ++i) {
        int eid = m_studentsTable->item(i, 0)->text().toInt();
        Enrollment e;
        e.setId(eid);
        e.setAttendanceCount(m_studentsTable->item(i, 2)->text().toInt());
        e.setAbsenceCount(m_studentsTable->item(i, 3)->text().toInt());
        e.setAssignment1Grade(m_studentsTable->item(i, 4)->text().toDouble());
        e.setAssignment2Grade(m_studentsTable->item(i, 5)->text().toDouble());
        e.setCourseworkGrade(m_studentsTable->item(i, 6)->text().toDouble());
        e.setFinalExamGrade(m_studentsTable->item(i, 7)->text().toDouble());
        
        m_enrollmentController.calculateTotalAndGrade(e, course.courseType(), course.maxGrade());
        if (!m_enrollmentController.updateEnrollment(e)) {
            success = false;
        }
    }
    
    if (success) {
        QMessageBox::information(this, "Success", "All student grades and attendance records updated successfully.");
    } else {
        QMessageBox::warning(this, "Partial Success", "Some records could not be updated. Please try again.");
    }
    onRefreshStudents();
}
