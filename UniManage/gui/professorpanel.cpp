#include "professorpanel.h"
#include "stylehelper.h"
#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QSet>
#include <algorithm>
#include <QSqlQuery>
#include <QSqlError>
#include "database/dbconnection.h"

/**
 * Constructor for the ProfessorPanel class
 * Initializes the professor panel with user data and UI components
 * @param userId - The user ID of the logged-in professor
 * @param parent - Parent widget (default nullptr)
 */
ProfessorPanel::ProfessorPanel(int userId, QWidget *parent)
    : QWidget(parent), m_userId(userId), 
      m_tabWidget(nullptr), m_yearSelector(nullptr), m_courseSelector(nullptr),
      m_as1Table(nullptr), m_as2Table(nullptr), m_cwTable(nullptr), 
      m_finalTable(nullptr), m_expTable(nullptr), m_evaluationTable(nullptr),
      m_noStudentsLabelGrades(nullptr), m_attendanceDate(nullptr), 
      m_attendanceTable(nullptr), m_noStudentsLabelAtt(nullptr),
      m_scheduleTable(nullptr), m_coursesTable(nullptr), 
      m_profSpecializationEdit(nullptr), m_profInfoEdit(nullptr), 
      m_calendarTable(nullptr)
{
    setStyleSheet(StyleHelper::getMainStyle());
    loadProfessorData();
    setupUI();
    loadCourses(); // Load lists
    loadSchedule();
    setWindowTitle("Professor Portal - Professor " + m_professor.fullName());
    resize(1200, 800);
}

/**
 * Destructor for the ProfessorPanel class
 * Cleans up resources when the professor panel is destroyed
 */
ProfessorPanel::~ProfessorPanel() {}

/**
 * Loads the professor's data from the database
 * Retrieves professor information using the user ID
 */
void ProfessorPanel::loadProfessorData() {
    m_professor = m_professorController.getProfessorByUserId(m_userId);
}

/**
 * Sets up the user interface for the professor panel
 * Creates tabs for grading, attendance, courses, schedule, calendar, and profile
 * Initializes filter controls for year and course selection
 */
void ProfessorPanel::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20,20,20,20);
    mainLayout->setSpacing(15);

    // Header
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* title = new QLabel("Professor Dashboard");
    title->setObjectName("titleLabel");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50;");
    header->addWidget(title);
    header->addStretch();
    
    QLabel* userLabel = new QLabel("Welcome, Professor " + m_professor.fullName());
    userLabel->setStyleSheet("font-size: 16px; color: #555; margin-right: 10px;");
    header->addWidget(userLabel);

    QPushButton* refreshBtn = new QPushButton("Refresh");
    refreshBtn->setObjectName("secondaryBtn");
    connect(refreshBtn, &QPushButton::clicked, this, &ProfessorPanel::onRefreshAll);
    header->addWidget(refreshBtn);

    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    connect(logoutBtn, &QPushButton::clicked, this, &ProfessorPanel::onLogout);
    header->addWidget(logoutBtn);
    
    mainLayout->addLayout(header);

    // Global Filter Bar
    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Academic Year:"));
    m_yearSelector = new QComboBox();
    m_yearSelector->setMinimumWidth(150);
    connect(m_yearSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfessorPanel::onYearSelected);
    filterLayout->addWidget(m_yearSelector);

    filterLayout->addSpacing(20);
    filterLayout->addWidget(new QLabel("Course:"));
    m_courseSelector = new QComboBox();
    m_courseSelector->setMinimumWidth(250);
    connect(m_courseSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfessorPanel::onCourseSelected);
    filterLayout->addWidget(m_courseSelector);

    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    // Context Tabs - Reorganized for better workflow
    m_tabWidget = new QTabWidget();
    m_tabWidget->addTab(createAs1Tab(), "As1");
    m_tabWidget->addTab(createAs2Tab(), "As2");
    m_tabWidget->addTab(createCWTab(), "CW");
    m_tabWidget->addTab(createFinalTab(), "Final");
    m_tabWidget->addTab(createExpTab(), "Experience");
    m_tabWidget->addTab(createEvaluationTab(), "Evaluation");
    
    m_tabWidget->addTab(createAttendanceTab(), "Attendance");
    m_tabWidget->addTab(createCoursesTab(), "My Courses");
    m_tabWidget->addTab(createScheduleTab(), "Schedule");
    m_tabWidget->addTab(createCalendarTab(), "Calendar");
    m_tabWidget->addTab(createProfileTab(), "Profile");

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &ProfessorPanel::onRefreshStudents);

    mainLayout->addWidget(m_tabWidget);
}


/**
 * Creates the Assignment 1 grading tab
 * Displays a table of students and their grades for Assignment 1
 * @return Pointer to the Assignment 1 tab widget
 */
QWidget* ProfessorPanel::createAs1Tab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    QPushButton* saveBtn = new QPushButton("Update Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onUpdateClicked);
    controls->addWidget(saveBtn);
    layout->addLayout(controls);
    
    m_as1Table = new QTableWidget();
    m_as1Table->setColumnCount(4);
    m_as1Table->setHorizontalHeaderLabels({"Code", "Name", "As1 Grade", "Action"});
    m_as1Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_as1Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_as1Table->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_as1Table, &QTableWidget::itemDoubleClicked, this, &ProfessorPanel::onUpdateClicked);
    layout->addWidget(m_as1Table);
    return tab;
}

/**
 * Creates the Assignment 2 grading tab
 * Displays a table of students and their grades for Assignment 2
 * @return Pointer to the Assignment 2 tab widget
 */
QWidget* ProfessorPanel::createAs2Tab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    QPushButton* saveBtn = new QPushButton("Update Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onUpdateClicked);
    controls->addWidget(saveBtn);
    layout->addLayout(controls);
    
    m_as2Table = new QTableWidget();
    m_as2Table->setColumnCount(4);
    m_as2Table->setHorizontalHeaderLabels({"Code", "Name", "As2 Grade", "Action"});
    m_as2Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_as2Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_as2Table->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_as2Table, &QTableWidget::itemDoubleClicked, this, &ProfessorPanel::onUpdateClicked);
    layout->addWidget(m_as2Table);
    return tab;
}

/**
 * Creates the Coursework grading tab
 * Displays a table of students and their grades for Coursework
 * @return Pointer to the Coursework tab widget
 */
QWidget* ProfessorPanel::createCWTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    QPushButton* saveBtn = new QPushButton("Update Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onUpdateClicked);
    controls->addWidget(saveBtn);
    layout->addLayout(controls);
    
    m_cwTable = new QTableWidget();
    m_cwTable->setColumnCount(4);
    m_cwTable->setHorizontalHeaderLabels({"Code", "Name", "CW Grade", "Action"});
    m_cwTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_cwTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_cwTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_cwTable, &QTableWidget::itemDoubleClicked, this, &ProfessorPanel::onUpdateClicked);
    layout->addWidget(m_cwTable);
    return tab;
}

/**
 * Creates the Final Exam grading tab
 * Displays a table of students and their grades for Final Exam
 * @return Pointer to the Final Exam tab widget
 */
QWidget* ProfessorPanel::createFinalTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    QPushButton* saveBtn = new QPushButton("Update Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onUpdateClicked);
    controls->addWidget(saveBtn);
    layout->addLayout(controls);
    
    m_finalTable = new QTableWidget();
    m_finalTable->setColumnCount(4);
    m_finalTable->setHorizontalHeaderLabels({"Code", "Name", "Final Grade", "Action"});
    m_finalTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_finalTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_finalTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_finalTable, &QTableWidget::itemDoubleClicked, this, &ProfessorPanel::onUpdateClicked);
    layout->addWidget(m_finalTable);
    return tab;
}

/**
 * Creates the Experience grading tab
 * Displays a table of students and their experience/practical grades
 * @return Pointer to the Experience tab widget
 */
QWidget* ProfessorPanel::createExpTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    QPushButton* saveBtn = new QPushButton("Update Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onUpdateClicked);
    controls->addWidget(saveBtn);
    layout->addLayout(controls);
    
    m_expTable = new QTableWidget();
    m_expTable->setColumnCount(4);
    m_expTable->setHorizontalHeaderLabels({"Code", "Name", "Exp. Grade", "Action"});
    m_expTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_expTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_expTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_expTable, &QTableWidget::itemDoubleClicked, this, &ProfessorPanel::onUpdateClicked);
    layout->addWidget(m_expTable);
    return tab;
}

/**
 * Creates the Comprehensive Evaluation tab
 * Displays a detailed table of students with all grade components and attendance stats
 * @return Pointer to the Evaluation tab widget
 */
QWidget* ProfessorPanel::createEvaluationTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Tools for Evaluation
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    
    QPushButton* refreshBtn = new QPushButton("Refresh List");
    connect(refreshBtn, &QPushButton::clicked, this, &ProfessorPanel::onRefreshStudents);
    controls->addWidget(refreshBtn);

    QPushButton* saveBtn = new QPushButton("Update Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onUpdateClicked);
    controls->addWidget(saveBtn);
    
    layout->addLayout(controls);

    // Evaluation Table
    m_evaluationTable = new QTableWidget();
    QStringList headers = {"Code", "Name", "Year", "Sec", "Att", "Abs", "As1", "As2", "CW", "Final", "Exp", "Total", "Grade", "Action"};
    m_evaluationTable->setColumnCount(headers.size());
    m_evaluationTable->setHorizontalHeaderLabels(headers);
    m_evaluationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_evaluationTable->setAlternatingRowColors(true);
    m_evaluationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_evaluationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_evaluationTable, &QTableWidget::itemDoubleClicked, this, &ProfessorPanel::onUpdateClicked);
    
    layout->addWidget(m_evaluationTable);

    m_noStudentsLabelGrades = new QLabel("No students found for the selected Course and Year.");
    m_noStudentsLabelGrades->setAlignment(Qt::AlignCenter);
    m_noStudentsLabelGrades->setStyleSheet("font-size: 16px; color: #7f8c8d; margin-top: 20px;");
    m_noStudentsLabelGrades->setVisible(false);
    layout->addWidget(m_noStudentsLabelGrades);

    return tab;
}

/**
 * Creates the Attendance Management tab
 * Allows professors to view and record daily attendance for courses
 * @return Pointer to the Attendance tab widget
 */
QWidget* ProfessorPanel::createAttendanceTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addWidget(new QLabel("Select Date:"));
    m_attendanceDate = new QDateEdit(QDate::currentDate());
    m_attendanceDate->setCalendarPopup(true);
    m_attendanceDate->setMinimumWidth(120);
    connect(m_attendanceDate, &QDateEdit::dateChanged, this, &ProfessorPanel::onRefreshStudents);
    controls->addWidget(m_attendanceDate);
    
    controls->addStretch();
    
    QPushButton* submitAttBtn = new QPushButton("Submit Attendance");
    submitAttBtn->setObjectName("primaryBtn");
    connect(submitAttBtn, &QPushButton::clicked, this, &ProfessorPanel::onSubmitAttendance);
    controls->addWidget(submitAttBtn);
    
    layout->addLayout(controls);

    // Attendance Table: Code, Name, Section, Today Status
    m_attendanceTable = new QTableWidget();
    QStringList headers = {"Student Code", "Student Name", "Year", "Section", "Today Status", "Notes"};
    m_attendanceTable->setColumnCount(headers.size());
    m_attendanceTable->setHorizontalHeaderLabels(headers);
    m_attendanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_attendanceTable->setAlternatingRowColors(true);
    layout->addWidget(m_attendanceTable);
    
    m_noStudentsLabelAtt = new QLabel("No students found.");
    m_noStudentsLabelAtt->setAlignment(Qt::AlignCenter);
    m_noStudentsLabelAtt->setStyleSheet("font-size: 16px; color: #7f8c8d; margin-top: 20px;");
    m_noStudentsLabelAtt->setVisible(false);
    layout->addWidget(m_noStudentsLabelAtt);

    return tab;
}


/**
 * Creates the Schedule View tab
 * Displays the professor's weekly teaching schedule
 * @return Pointer to the Schedule tab widget
 */
QWidget* ProfessorPanel::createScheduleTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    m_scheduleTable = new QTableWidget();
    m_scheduleTable->setColumnCount(5);
    m_scheduleTable->setHorizontalHeaderLabels({"Day", "Time", "Course", "Room", "Type"});
    m_scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_scheduleTable);
    
    return tab;
}

/**
 * Creates the Courses List tab
 * Displays all courses assigned to the professor with details
 * @return Pointer to the Courses tab widget
 */
QWidget* ProfessorPanel::createCoursesTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QLabel* infoLabel = new QLabel("Courses Assigned to You:");
    infoLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px;");
    layout->addWidget(infoLabel);

    m_coursesTable = new QTableWidget();
    m_coursesTable->setColumnCount(6);
    m_coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Desc", "Credits", "Type", "Max Grade"});
    m_coursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only
    layout->addWidget(m_coursesTable);
    
    QLabel* noteLabel = new QLabel("Note: Courses are assigned by the administrator. Contact admin to modify course assignments.");
    noteLabel->setStyleSheet("color: #666; font-style: italic; padding: 5px;");
    layout->addWidget(noteLabel);
    
    return tab;
}

/**
 * Creates the Profile Management tab
 * Allows professors to update their specialization and personal info
 * @return Pointer to the Profile tab widget
 */
QWidget* ProfessorPanel::createProfileTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QFormLayout* form = new QFormLayout();
    
    m_profSpecializationEdit = new QLineEdit();
    m_profInfoEdit = new QTextEdit(); // HTML capable
    
    m_profSpecializationEdit->setText(m_professor.specialization());
    m_profInfoEdit->setHtml(m_professor.personalInfo());
    
    form->addRow("Specialization:", m_profSpecializationEdit);
    form->addRow("Personal Info:", m_profInfoEdit);
    
    layout->addLayout(form);
    
    QPushButton* saveBtn = new QPushButton("Update Profile");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onSaveProfile);
    layout->addWidget(saveBtn, 0, Qt::AlignRight);
    
    layout->addStretch();
    return tab;
}

/**
 * Creates the Calendar & Events tab
 * Allows professors to manage academic events and view the calendar
 * @return Pointer to the Calendar tab widget
 */
QWidget* ProfessorPanel::createCalendarTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QLabel* titleLabel = new QLabel("Academic Calendar & Events");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 5px;");
    layout->addWidget(titleLabel);
    
    // Controls for calendar management
    QHBoxLayout* controls = new QHBoxLayout();
    
    QPushButton* addEventBtn = new QPushButton("Add Event");
    addEventBtn->setObjectName("primaryBtn");
    connect(addEventBtn, &QPushButton::clicked, this, &ProfessorPanel::onAddCalendarEvent);
    controls->addWidget(addEventBtn);
    
    QPushButton* editEventBtn = new QPushButton("Edit Event");
    editEventBtn->setObjectName("secondaryBtn");
    connect(editEventBtn, &QPushButton::clicked, this, &ProfessorPanel::onEditCalendarEvent);
    controls->addWidget(editEventBtn);
    
    QPushButton* deleteEventBtn = new QPushButton("Delete Event");
    deleteEventBtn->setObjectName("dangerBtn");
    connect(deleteEventBtn, &QPushButton::clicked, this, &ProfessorPanel::onDeleteCalendarEvent);
    controls->addWidget(deleteEventBtn);
    
    controls->addStretch();
    
    QPushButton* refreshBtn = new QPushButton("Refresh");
    connect(refreshBtn, &QPushButton::clicked, this, &ProfessorPanel::loadCalendarEvents);
    controls->addWidget(refreshBtn);
    
    layout->addLayout(controls);
    
    // Calendar events table
    m_calendarTable = new QTableWidget();
    m_calendarTable->setColumnCount(6);
    m_calendarTable->setHorizontalHeaderLabels({"ID", "Title", "Start Date", "End Date", "Type", "Description"});
    m_calendarTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_calendarTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_calendarTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_calendarTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_calendarTable->setAlternatingRowColors(true);
    layout->addWidget(m_calendarTable);
    
    loadCalendarEvents();
    
    return tab;
}

/**
 * Handles the logout button click event
 * Closes the professor panel and returns to the login screen
 */
void ProfessorPanel::onLogout() {
    LoginWindow* win = new LoginWindow();
    win->show();
    this->close();
}

/**
 * Refreshes all data in the professor panel
 * Reloads professor data, courses, schedule, calendar, and student lists
 */
void ProfessorPanel::onRefreshAll() {
    loadProfessorData();
    loadCourses(); 
    loadSchedule();
    loadCalendarEvents();
    onRefreshStudents(); 
    QMessageBox::information(this, "Refreshed", "All data reloaded from database.");
}

/**
 * Loads the courses assigned to the professor
 * Updates the course selection dropdown based on selected academic year
 */
void ProfessorPanel::loadCourses() {
    m_allProfessorCourses = m_courseController.getCoursesByProfessor(m_professor.id());
    
    m_yearSelector->blockSignals(true);
    m_yearSelector->clear();
    
    QList<AcademicLevel> levels = m_academicLevelController.getAllAcademicLevels();
    for(const auto& l : levels) {
        m_yearSelector->addItem(l.name(), l.id());
    }
    
    m_yearSelector->blockSignals(false);
    
    // Refresh Courses Table (all assigned courses)
    m_coursesTable->setRowCount(0);
    for(const auto& c : m_allProfessorCourses) {
        int r = m_coursesTable->rowCount();
        m_coursesTable->insertRow(r);
        m_coursesTable->setItem(r, 0, new QTableWidgetItem(QString::number(c.id())));
        m_coursesTable->setItem(r, 1, new QTableWidgetItem(c.name()));
        m_coursesTable->setItem(r, 2, new QTableWidgetItem(c.description()));
        m_coursesTable->setItem(r, 3, new QTableWidgetItem(QString::number(c.creditHours())));
        m_coursesTable->setItem(r, 4, new QTableWidgetItem(c.courseType()));
        m_coursesTable->setItem(r, 5, new QTableWidgetItem(QString::number(c.maxGrade())));
    }
    
    if(m_yearSelector->count() > 0) {
        m_yearSelector->setCurrentIndex(0);
        onYearSelected(0);
    }
}

/**
 * Handles selection of an academic year/level
 * Filters the course list to show only courses for the selected level
 * @param index - Index of the selected year in the dropdown
 */
void ProfessorPanel::onYearSelected(int index) {
    if (index < 0) return;
    int selectedLevelId = m_yearSelector->currentData().toInt();
    
    m_courseSelector->blockSignals(true);
    m_courseSelector->clear();
    for(const auto& c : m_allProfessorCourses) {
        // Now comparing course's year_level (which stores level ID) with the selected level ID
        if (c.yearLevel() == selectedLevelId) {
            m_courseSelector->addItem(c.name(), c.id());
        }
    }
    m_courseSelector->blockSignals(false);
    
    if (m_courseSelector->count() > 0) {
        m_courseSelector->setCurrentIndex(0);
    }
    onRefreshStudents();
}

/**
 * Loads the professor's schedule
 * Retreives schedule data and populates the schedule table
 */
void ProfessorPanel::loadSchedule() {
    m_scheduleTable->setRowCount(0);
    QList<Schedule> scheds = m_scheduleController.getScheduleByProfessor(m_professor.id());
    for(const auto& s : scheds) {
        int r = m_scheduleTable->rowCount();
        m_scheduleTable->insertRow(r);
        m_scheduleTable->setItem(r, 0, new QTableWidgetItem(s.dayOfWeek()));
        m_scheduleTable->setItem(r, 1, new QTableWidgetItem(s.startTime().toString("HH:mm") + " - " + s.endTime().toString("HH:mm")));
        m_scheduleTable->setItem(r, 2, new QTableWidgetItem(s.courseName()));
        m_scheduleTable->setItem(r, 3, new QTableWidgetItem(s.roomName()));
        m_scheduleTable->setItem(r, 4, new QTableWidgetItem("Assigned Session"));
    }
}

/**
 * Loads calendar events
 * Retrieves all events and populates the calendar table
 */
void ProfessorPanel::loadCalendarEvents() {
    m_calendarTable->setRowCount(0);
    for(const auto& e : m_calendarController.getAllEvents()) {
        int r = m_calendarTable->rowCount();
        m_calendarTable->insertRow(r);
        m_calendarTable->setItem(r, 0, new QTableWidgetItem(QString::number(e.id())));
        m_calendarTable->setItem(r, 1, new QTableWidgetItem(e.title()));
        m_calendarTable->setItem(r, 2, new QTableWidgetItem(e.startDate().toString("yyyy-MM-dd")));
        m_calendarTable->setItem(r, 3, new QTableWidgetItem(e.endDate().toString("yyyy-MM-dd")));
        m_calendarTable->setItem(r, 4, new QTableWidgetItem(e.eventType()));
        m_calendarTable->setItem(r, 5, new QTableWidgetItem(e.description()));
    }
}

/**
 * Handles selection of a course
 * Triggers a refresh of student data for the selected course
 * @param index - Index of the selected course in the dropdown
 */
void ProfessorPanel::onCourseSelected(int index) {
    onRefreshStudents();
}

/**
 * Refreshes the list of students for the selected course
 * Updates all grading tables and the attendance table with enrolled students
 * Fetches current grades and attendance status
 */
void ProfessorPanel::onRefreshStudents() {
    if(m_courseSelector->count() == 0) {
        m_as1Table->setRowCount(0);
        m_as2Table->setRowCount(0);
        m_cwTable->setRowCount(0);
        m_finalTable->setRowCount(0);
        m_expTable->setRowCount(0);
        m_evaluationTable->setRowCount(0);
        m_attendanceTable->setRowCount(0);
        return;
    }

    int courseId = m_courseSelector->currentData().toInt();
    QDate currentAttDate = m_attendanceDate->date();
    
    QList<Enrollment> enrollments = m_enrollmentController.getEnrollmentsByCourse(courseId);
    QList<AttendanceLog> logs = m_enrollmentController.getAttendanceLogsByCourse(courseId, currentAttDate);
    
    QList<Enrollment> filtered = enrollments; // Show all students for the selected course
    
    bool hasStudents = !filtered.isEmpty();

    // Reset all tables
    m_as1Table->setRowCount(0);
    m_as2Table->setRowCount(0);
    m_cwTable->setRowCount(0);
    m_finalTable->setRowCount(0);
    m_expTable->setRowCount(0);
    m_evaluationTable->setRowCount(0);
    
    m_noStudentsLabelGrades->setVisible(!hasStudents);

    for(const auto& e : filtered) {
        // AS1
        int r1 = m_as1Table->rowCount();
        m_as1Table->insertRow(r1);
        QTableWidgetItem* iCode1 = new QTableWidgetItem(e.studentCode());
        if(iCode1) {
            iCode1->setData(Qt::UserRole, e.id());
            iCode1->setFlags(iCode1->flags().setFlag(Qt::ItemIsEditable, false));
            m_as1Table->setItem(r1, 0, iCode1);
        }
        m_as1Table->setItem(r1, 1, new QTableWidgetItem(e.studentName()));
        if(m_as1Table->item(r1, 1)) m_as1Table->item(r1, 1)->setFlags(m_as1Table->item(r1, 1)->flags().setFlag(Qt::ItemIsEditable, false));
        m_as1Table->setItem(r1, 2, new QTableWidgetItem(QString::number(e.assignment1Grade())));

        // AS2
        int r2 = m_as2Table->rowCount();
        m_as2Table->insertRow(r2);
        QTableWidgetItem* iCode2 = new QTableWidgetItem(e.studentCode());
        if(iCode2) {
            iCode2->setData(Qt::UserRole, e.id());
            iCode2->setFlags(iCode2->flags().setFlag(Qt::ItemIsEditable, false));
            m_as2Table->setItem(r2, 0, iCode2);
        }
        m_as2Table->setItem(r2, 1, new QTableWidgetItem(e.studentName()));
        if(m_as2Table->item(r2, 1)) m_as2Table->item(r2, 1)->setFlags(m_as2Table->item(r2, 1)->flags().setFlag(Qt::ItemIsEditable, false));
        m_as2Table->setItem(r2, 2, new QTableWidgetItem(QString::number(e.assignment2Grade())));

        // CW
        int r3 = m_cwTable->rowCount();
        m_cwTable->insertRow(r3);
        QTableWidgetItem* iCode3 = new QTableWidgetItem(e.studentCode());
        if(iCode3) {
            iCode3->setData(Qt::UserRole, e.id());
            iCode3->setFlags(iCode3->flags().setFlag(Qt::ItemIsEditable, false));
            m_cwTable->setItem(r3, 0, iCode3);
        }
        m_cwTable->setItem(r3, 1, new QTableWidgetItem(e.studentName()));
        if(m_cwTable->item(r3, 1)) m_cwTable->item(r3, 1)->setFlags(m_cwTable->item(r3, 1)->flags().setFlag(Qt::ItemIsEditable, false));
        m_cwTable->setItem(r3, 2, new QTableWidgetItem(QString::number(e.courseworkGrade())));

        // Final
        int r4 = m_finalTable->rowCount();
        m_finalTable->insertRow(r4);
        QTableWidgetItem* iCode4 = new QTableWidgetItem(e.studentCode());
        if(iCode4) {
            iCode4->setData(Qt::UserRole, e.id());
            iCode4->setFlags(iCode4->flags().setFlag(Qt::ItemIsEditable, false));
            m_finalTable->setItem(r4, 0, iCode4);
        }
        m_finalTable->setItem(r4, 1, new QTableWidgetItem(e.studentName()));
        if(m_finalTable->item(r4, 1)) m_finalTable->item(r4, 1)->setFlags(m_finalTable->item(r4, 1)->flags().setFlag(Qt::ItemIsEditable, false));
        m_finalTable->setItem(r4, 2, new QTableWidgetItem(QString::number(e.finalExamGrade())));

        // Exp
        int r5 = m_expTable->rowCount();
        m_expTable->insertRow(r5);
        QTableWidgetItem* iCode5 = new QTableWidgetItem(e.studentCode());
        if(iCode5) {
            iCode5->setData(Qt::UserRole, e.id());
            iCode5->setFlags(iCode5->flags().setFlag(Qt::ItemIsEditable, false));
            m_expTable->setItem(r5, 0, iCode5);
        }
        m_expTable->setItem(r5, 1, new QTableWidgetItem(e.studentName()));
        if(m_expTable->item(r5, 1)) m_expTable->item(r5, 1)->setFlags(m_expTable->item(r5, 1)->flags().setFlag(Qt::ItemIsEditable, false));
        m_expTable->setItem(r5, 2, new QTableWidgetItem(QString::number(e.experienceGrade())));

        // Evaluation
        int r6 = m_evaluationTable->rowCount();
        m_evaluationTable->insertRow(r6);
        QTableWidgetItem* iCode6 = new QTableWidgetItem(e.studentCode());
        if(iCode6) {
            iCode6->setData(Qt::UserRole, e.id());
            iCode6->setFlags(iCode6->flags().setFlag(Qt::ItemIsEditable, false));
            m_evaluationTable->setItem(r6, 0, iCode6);
        }
        m_evaluationTable->setItem(r6, 1, new QTableWidgetItem(e.studentName()));
        if(m_evaluationTable->item(r6, 1)) m_evaluationTable->item(r6, 1)->setFlags(m_evaluationTable->item(r6, 1)->flags().setFlag(Qt::ItemIsEditable, false));
        m_evaluationTable->setItem(r6, 2, new QTableWidgetItem(e.academicYear()));
        if(m_evaluationTable->item(r6, 2)) m_evaluationTable->item(r6, 2)->setFlags(m_evaluationTable->item(r6, 2)->flags().setFlag(Qt::ItemIsEditable, false));
        m_evaluationTable->setItem(r6, 3, new QTableWidgetItem(e.studentSection()));
        if(m_evaluationTable->item(r6, 3)) m_evaluationTable->item(r6, 3)->setFlags(m_evaluationTable->item(r6, 3)->flags().setFlag(Qt::ItemIsEditable, false));
        m_evaluationTable->setItem(r6, 4, new QTableWidgetItem(QString::number(e.attendanceCount())));
        if(m_evaluationTable->item(r6, 4)) m_evaluationTable->item(r6, 4)->setFlags(m_evaluationTable->item(r6, 4)->flags().setFlag(Qt::ItemIsEditable, false));
        m_evaluationTable->setItem(r6, 5, new QTableWidgetItem(QString::number(e.absenceCount())));
        if(m_evaluationTable->item(r6, 5)) m_evaluationTable->item(r6, 5)->setFlags(m_evaluationTable->item(r6, 5)->flags().setFlag(Qt::ItemIsEditable, false));
        m_evaluationTable->setItem(r6, 6, new QTableWidgetItem(QString::number(e.assignment1Grade())));
        m_evaluationTable->setItem(r6, 7, new QTableWidgetItem(QString::number(e.assignment2Grade())));
        m_evaluationTable->setItem(r6, 8, new QTableWidgetItem(QString::number(e.courseworkGrade())));
        m_evaluationTable->setItem(r6, 9, new QTableWidgetItem(QString::number(e.finalExamGrade())));
        m_evaluationTable->setItem(r6, 10, new QTableWidgetItem(QString::number(e.experienceGrade())));
        m_evaluationTable->setItem(r6, 11, new QTableWidgetItem(QString::number(e.totalGrade())));
        if(m_evaluationTable->item(r6, 11)) m_evaluationTable->item(r6, 11)->setFlags(m_evaluationTable->item(r6, 11)->flags().setFlag(Qt::ItemIsEditable, false));
        m_evaluationTable->setItem(r6, 12, new QTableWidgetItem(e.letterGrade()));
        if(m_evaluationTable->item(r6, 12)) m_evaluationTable->item(r6, 12)->setFlags(m_evaluationTable->item(r6, 12)->flags().setFlag(Qt::ItemIsEditable, false));

        // Add Edit Buttons to all tables
        auto addEditBtn = [this](QTableWidget* table, int row, int col, int eid, int tabMode) {
            QPushButton* btn = new QPushButton("Update");
            btn->setObjectName("actionBtn");
            connect(btn, &QPushButton::clicked, [this, eid, tabMode]() { showGradeForm(eid, tabMode); });
            table->setCellWidget(row, col, btn);
        };

        addEditBtn(m_as1Table, r1, 3, e.id(), 0);
        addEditBtn(m_as2Table, r2, 3, e.id(), 1);
        addEditBtn(m_cwTable, r3, 3, e.id(), 2);
        addEditBtn(m_finalTable, r4, 3, e.id(), 3);
        addEditBtn(m_expTable, r5, 3, e.id(), 4);
        addEditBtn(m_evaluationTable, r6, 13, e.id(), 5);
    }

    // 2. Refresh Attendance Table
    if (m_attendanceTable && m_attendanceTable->isVisible()) {
        m_attendanceTable->setRowCount(0);
        m_noStudentsLabelAtt->setVisible(!hasStudents);
        m_attendanceTable->setVisible(hasStudents);

        if(hasStudents) {
            for(const auto& e : enrollments) {
                int r = m_attendanceTable->rowCount();
                m_attendanceTable->insertRow(r);
                
                // Code, Name, Section, Today Status, Notes
                QTableWidgetItem* iCode = new QTableWidgetItem(e.studentCode());
                iCode->setData(Qt::UserRole, e.id());
                iCode->setFlags(iCode->flags() & ~Qt::ItemIsEditable);
                m_attendanceTable->setItem(r, 0, iCode);
                
                QTableWidgetItem* iName = new QTableWidgetItem(e.studentName());
                iName->setFlags(iName->flags() & ~Qt::ItemIsEditable);
                m_attendanceTable->setItem(r, 1, iName);
                
                QTableWidgetItem* iYear = new QTableWidgetItem(e.academicYear());
                iYear->setFlags(iYear->flags() & ~Qt::ItemIsEditable);
                m_attendanceTable->setItem(r, 2, iYear);

                QTableWidgetItem* iSec = new QTableWidgetItem(e.studentSection());
                iSec->setFlags(iSec->flags() & ~Qt::ItemIsEditable);
                m_attendanceTable->setItem(r, 3, iSec);
                
                QComboBox* cb = new QComboBox();
                cb->addItems({"Present", "Absent", "Late", "Excused"});
                
                // Find existing log
                QString status = "Present"; // Default? Or empty?
                bool found = false;
                for(const auto& log : logs) {
                    if(log.enrollmentId() == e.id()) {
                        status = log.status();
                        found = true;
                        break;
                    }
                }
                if(found) cb->setCurrentText(status);
                else cb->setCurrentIndex(-1); // No selection
                cb->setPlaceholderText("Select");
                
                m_attendanceTable->setCellWidget(r, 4, cb);
                m_attendanceTable->setItem(r, 5, new QTableWidgetItem("")); // Notes placeholder
            }
        }
    }
}

/**
 * Submits attendance records for the selected course and date
 * iteratest through the attendance table and saves the status for each student
 */
void ProfessorPanel::onSubmitAttendance() {
    if(m_courseSelector->count() == 0) return;
    if(!m_attendanceTable) return;

    QDate date = m_attendanceDate->date();
    
    bool allOk = true;
    for(int i=0; i<m_attendanceTable->rowCount(); ++i) {
        int eid = m_attendanceTable->item(i, 0)->data(Qt::UserRole).toInt();
        QComboBox* cb = qobject_cast<QComboBox*>(m_attendanceTable->cellWidget(i, 4)); // Col 4 is Status
        QString status = cb->currentText();
        
        if(status.isEmpty()) continue;
        
        AttendanceLog log;
        log.setEnrollmentId(eid);
        log.setDate(date);
        log.setStatus(status);
        log.setNotes("Logged via Professor Panel");
        
        if(!m_enrollmentController.addAttendanceLog(log)) {
            allOk = false;
        }
    }
    
    if(allOk) {
        QMessageBox::information(this, "Success", "Attendance updated successfully.");
        onRefreshStudents();
    } else {
        QMessageBox::warning(this, "Partial Success", "Some records could not be updated.");
    }
}

/**
 * Handles the click event for updating grades
 * Identifies the active tab and selected student, then opens the grade entry form
 */
void ProfessorPanel::onUpdateClicked() {
    int tabIdx = m_tabWidget->currentIndex();
    QTableWidget* table = nullptr;
    if(tabIdx == 0) table = m_as1Table;
    else if(tabIdx == 1) table = m_as2Table;
    else if(tabIdx == 2) table = m_cwTable;
    else if(tabIdx == 3) table = m_finalTable;
    else if(tabIdx == 4) table = m_expTable;
    else if(tabIdx == 5) table = m_evaluationTable;

    if(!table) return;
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Select Student", "Please select a student from the list first (click a row).");
        return;
    }
    
    QTableWidgetItem* idItem = table->item(row, 0);
    if(!idItem) return;
    int eid = idItem->data(Qt::UserRole).toInt();
    showGradeForm(eid, tabIdx);
}

/**
 * Displays a dialog for entering/updating grades for a student
 * @param enrollmentId - The enrollment ID of the student
 * @param tabIndex - The index of the tab triggering the update (determines which fields are shown)
 */
void ProfessorPanel::showGradeForm(int enrollmentId, int tabIndex) {
    Enrollment e = m_enrollmentController.getEnrollmentById(enrollmentId);
    if(e.id() == 0) {
        QMessageBox::critical(this, "Error", "Failed to retrieve student record.");
        return;
    }

    QDialog dlg(this);
    QString baseTitle = "Update Grade";
    if(tabIndex == 0) baseTitle = "Update Assignment 1";
    else if(tabIndex == 1) baseTitle = "Update Assignment 2";
    else if(tabIndex == 2) baseTitle = "Update Coursework";
    else if(tabIndex == 3) baseTitle = "Update Final Exam";
    else if(tabIndex == 4) baseTitle = "Update Experience";
    else if(tabIndex == 5) baseTitle = "Full Evaluation";

    dlg.setWindowTitle(baseTitle + " - " + e.studentName());
    dlg.setMinimumWidth(400);
    QVBoxLayout* layout = new QVBoxLayout(&dlg);
    
    QFormLayout* form = new QFormLayout();
    
    QLineEdit* as1Ed = new QLineEdit(QString::number(e.assignment1Grade()));
    QLineEdit* as2Ed = new QLineEdit(QString::number(e.assignment2Grade()));
    QLineEdit* cwEd = new QLineEdit(QString::number(e.courseworkGrade()));
    QLineEdit* finalEd = new QLineEdit(QString::number(e.finalExamGrade()));
    QLineEdit* expEd = new QLineEdit(QString::number(e.experienceGrade()));

    // Show only the relevant field for the selected tab
    if(tabIndex == 0 || tabIndex == 5) form->addRow("Assignment 1 Grade:", as1Ed);
    if(tabIndex == 1 || tabIndex == 5) form->addRow("Assignment 2 Grade:", as2Ed);
    if(tabIndex == 2 || tabIndex == 5) form->addRow("Coursework Grade:", cwEd);
    if(tabIndex == 3 || tabIndex == 5) form->addRow("Final Exam Grade:", finalEd);
    if(tabIndex == 4 || tabIndex == 5) form->addRow("Experience Grade:", expEd);
    
    layout->addLayout(form);

    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* saveBtn = new QPushButton("Save Grades");
    saveBtn->setObjectName("primaryBtn");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    
    buttons->addStretch();
    buttons->addWidget(saveBtn);
    buttons->addWidget(cancelBtn);
    layout->addLayout(buttons);

    connect(saveBtn, &QPushButton::clicked, [this, &dlg, e, as1Ed, as2Ed, cwEd, finalEd, expEd]() mutable {
        e.setAssignment1Grade(as1Ed->text().toDouble());
        e.setAssignment2Grade(as2Ed->text().toDouble());
        e.setCourseworkGrade(cwEd->text().toDouble());
        e.setFinalExamGrade(finalEd->text().toDouble());
        e.setExperienceGrade(expEd->text().toDouble());

        int cid = m_courseSelector->currentData().toInt();
        if(cid <= 0) {
             QMessageBox::warning(&dlg, "Selection Error", "Please select a course first.");
             return;
        }
        Course c = m_courseController.getCourseById(cid);
        m_enrollmentController.calculateTotalAndGrade(e, c.courseType(), c.maxGrade());
        
        if(m_enrollmentController.updateEnrollment(e)) {
            QMessageBox::information(&dlg, "Success", "Grades updated successfully.");
            dlg.accept();
            onRefreshStudents();
        } else {
            QString error = DBConnection::instance().database().lastError().text();
            QMessageBox::critical(&dlg, "Database Error", 
                "Failed to update grades in database.\n\n"
                "Error Details: " + (error.isEmpty() ? "Unknown Error" : error));
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    dlg.exec();
}


/**
 * Opens a dialog to add a new calendar event
 * Saves the new event to the database upon confirmation
 */
void ProfessorPanel::onAddCalendarEvent() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add Event");
    QFormLayout* form = new QFormLayout(&dlg);
    
    QLineEdit* titleEd = new QLineEdit();
    QTextEdit* descEd = new QTextEdit();
    QDateEdit* startDe = new QDateEdit(QDate::currentDate());
    QDateEdit* endDe = new QDateEdit(QDate::currentDate());
    QComboBox* typeCb = new QComboBox();
    typeCb->addItems({"Academic", "Holiday", "Exam", "Registration", "Other"});
    
    form->addRow("Title:", titleEd);
    form->addRow("Description:", descEd);
    form->addRow("Start Date:", startDe);
    form->addRow("End Date:", endDe);
    form->addRow("Event Type:", typeCb);
    
    QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    form->addRow(bb);
    
    if(dlg.exec() == QDialog::Accepted) {
        CalendarEvent e;
        e.setTitle(titleEd->text());
        e.setDescription(descEd->toPlainText());
        e.setStartDate(startDe->date());
        e.setEndDate(endDe->date());
        e.setEventType(typeCb->currentText());
        
        if(m_calendarController.addEvent(e)) {
            QMessageBox::information(this, "Success", "Event added.");
            loadCalendarEvents();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add event.");
        }
    }
}

/**
 * Opens a dialog to edit an existing calendar event
 * Updates the event in the database upon confirmation
 */
void ProfessorPanel::onEditCalendarEvent() {
    int cur = m_calendarTable->currentRow();
    if(cur < 0) return;
    
    int eid = m_calendarTable->item(cur, 0)->text().toInt();
    QList<CalendarEvent> all = m_calendarController.getAllEvents();
    CalendarEvent e;
    bool found = false;
    for(const auto& item : all) { if(item.id() == eid) { e = item; found = true; break; } }
    if(!found) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Edit Event");
    QFormLayout* form = new QFormLayout(&dlg);
    
    QLineEdit* titleEd = new QLineEdit(e.title());
    QTextEdit* descEd = new QTextEdit(e.description());
    QDateEdit* startDe = new QDateEdit(e.startDate());
    QDateEdit* endDe = new QDateEdit(e.endDate());
    QComboBox* typeCb = new QComboBox();
    typeCb->addItems({"Academic", "Holiday", "Exam", "Registration", "Other"});
    typeCb->setCurrentText(e.eventType());
    
    form->addRow("Title:", titleEd);
    form->addRow("Description:", descEd);
    form->addRow("Start Date:", startDe);
    form->addRow("End Date:", endDe);
    form->addRow("Event Type:", typeCb);
    
    QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    form->addRow(bb);
    
    if(dlg.exec() == QDialog::Accepted) {
        e.setTitle(titleEd->text());
        e.setDescription(descEd->toPlainText());
        e.setStartDate(startDe->date());
        e.setEndDate(endDe->date());
        e.setEventType(typeCb->currentText());
        
        if(m_calendarController.updateEvent(e)) {
            QMessageBox::information(this, "Success", "Event updated.");
            loadCalendarEvents();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update event.");
        }
    }
}

/**
 * Deletes the selected calendar event
 * Removes the event from the database after user confirmation
 */
void ProfessorPanel::onDeleteCalendarEvent() {
    int cur = m_calendarTable->currentRow();
    if(cur < 0) return;
    
    int eid = m_calendarTable->item(cur, 0)->text().toInt();
    if(QMessageBox::question(this, "Confirm", "Delete this event?") == QMessageBox::Yes) {
        if(m_calendarController.deleteEvent(eid)) {
            QMessageBox::information(this, "Success", "Event deleted.");
            loadCalendarEvents();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete event.");
        }
    }
}

/**
 * Saves changes to the professor's profile
 * Updates specialization and personal info in the database
 */
void ProfessorPanel::onSaveProfile() {
    m_professor.setSpecialization(m_profSpecializationEdit->text());
    m_professor.setPersonalInfo(m_profInfoEdit->toHtml());
    
    if(m_professorController.updateProfessor(m_professor)) {
        QMessageBox::information(this, "Success", "Profile updated.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update profile.");
    }
}
