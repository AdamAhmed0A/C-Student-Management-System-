#include "studentportal.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>

StudentPortal::StudentPortal(int studentId, QWidget *parent)
    : QWidget(parent), m_studentId(studentId)
{
    m_student = m_studentController.getStudentById(studentId);
    setupUI();
    setWindowTitle("Student Portal - University SIS");
    resize(900, 600);
    
    loadDashboard();
    loadCourses();
}

StudentPortal::~StudentPortal()
{
}

void StudentPortal::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QLabel* titleLabel = new QLabel("Student Portal", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createDashboardTab(), "Dashboard");
    m_tabWidget->addTab(createCoursesTab(), "My Courses");
    
    mainLayout->addWidget(m_tabWidget);
}

QWidget* StudentPortal::createDashboardTab()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QGroupBox* profileGroup = new QGroupBox("Profile Information", widget);
    QFormLayout* formLayout = new QFormLayout(profileGroup);
    
    m_studentNameLabel = new QLabel();
    m_studentNumberLabel = new QLabel();
    
    formLayout->addRow("Name:", m_studentNameLabel);
    formLayout->addRow("Student Number:", m_studentNumberLabel);
    
    layout->addWidget(profileGroup);
    layout->addStretch();
    
    return widget;
}

QWidget* StudentPortal::createCoursesTab()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QLabel* titleLabel = new QLabel("Enrolled Courses", widget);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);
    
    m_coursesTable = new QTableWidget();
    m_coursesTable->setColumnCount(4);
    m_coursesTable->setHorizontalHeaderLabels({"Course Name", "Description", "Year Level", "Credit Hours"});
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_coursesTable);
    
    return widget;
}

void StudentPortal::loadDashboard()
{
    m_studentNameLabel->setText(m_student.fullName());
    m_studentNumberLabel->setText(m_student.studentNumber());
}

void StudentPortal::loadCourses()
{
    m_coursesTable->setRowCount(0);
    // TODO: Load enrolled courses from database
}
