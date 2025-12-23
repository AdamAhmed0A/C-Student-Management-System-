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

ProfessorPanel::ProfessorPanel(int userId, QWidget *parent)
    : QWidget(parent), m_userId(userId)
{
    setStyleSheet(StyleHelper::getMainStyle());
    loadProfessorData();
    setupUI();
    loadCourses(); // Load lists
    loadSchedule();
    setWindowTitle("Professor Portal - Professor " + m_professor.fullName());
    resize(1200, 800);
}

ProfessorPanel::~ProfessorPanel() {}

void ProfessorPanel::loadProfessorData() {
    m_professor = m_professorController.getProfessorByUserId(m_userId);
}

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
    m_tabWidget->addTab(createGradesTab(), "Student Grades"); // Dashboard
    m_tabWidget->addTab(createAttendanceTab(), "Attendance");
    m_tabWidget->addTab(createCoursesTab(), "My Courses");
    m_tabWidget->addTab(createScheduleTab(), "Schedule");
    m_tabWidget->addTab(createCalendarTab(), "Calendar");
    m_tabWidget->addTab(createProfileTab(), "Profile");

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &ProfessorPanel::onRefreshStudents);

    mainLayout->addWidget(m_tabWidget);
}


QWidget* ProfessorPanel::createGradesTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Tools for Grades
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    
    QPushButton* refreshBtn = new QPushButton("Refresh List");
    connect(refreshBtn, &QPushButton::clicked, this, &ProfessorPanel::onRefreshStudents);
    controls->addWidget(refreshBtn);

    QPushButton* saveBtn = new QPushButton("Save Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onSaveGrades);
    controls->addWidget(saveBtn);
    
    layout->addLayout(controls);

    // Grades Table: Student Code, Name, Section, Attendance, Absences, Assignments...
    m_gradesTable = new QTableWidget();
    QStringList headers = {"Student Code", "Student Name", "Section", "Attendance", "Absences", "Ass. 1", "Ass. 2", "CW", "Final", "Total", "Grade"};
    m_gradesTable->setColumnCount(headers.size());
    m_gradesTable->setHorizontalHeaderLabels(headers);
    m_gradesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_gradesTable->setAlternatingRowColors(true);
    
    layout->addWidget(m_gradesTable);

    
    m_noStudentsLabelGrades = new QLabel("No students found for the selected Course and Year.");
    m_noStudentsLabelGrades->setAlignment(Qt::AlignCenter);
    m_noStudentsLabelGrades->setStyleSheet("font-size: 16px; color: #7f8c8d; margin-top: 20px;");
    m_noStudentsLabelGrades->setVisible(false);
    layout->addWidget(m_noStudentsLabelGrades);

    return tab;
}

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
    QStringList headers = {"Student Code", "Student Name", "Section", "Today Status", "Notes"};
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

void ProfessorPanel::onLogout() {
    LoginWindow* win = new LoginWindow();
    win->show();
    this->close();
}

void ProfessorPanel::onRefreshAll() {
    loadProfessorData();
    loadCourses(); 
    loadSchedule();
    loadCalendarEvents();
    onRefreshStudents(); 
    QMessageBox::information(this, "Refreshed", "All data reloaded from database.");
}

void ProfessorPanel::loadCourses() {
    m_allProfessorCourses = m_courseController.getCoursesByProfessor(m_professor.id());
    
    // Refresh Year Selector with Academic Levels (Names from DB)
    m_yearSelector->blockSignals(true);
    m_yearSelector->clear();
    
    QSet<int> uniqueLevelIds;
    for(const auto& c : m_allProfessorCourses) {
        uniqueLevelIds.insert(c.yearLevel()); // Using yearLevel() as ID (assuming enhancement follows valid FK usage)
    }
    
    // Fetch all levels to map ID -> Name
    QList<AcademicLevel> allLevels = m_academicLevelController.getAllAcademicLevels();
    
    // Filter only those relevant to assigned courses
    // We want to sort them by level number for display logic if possible, or just level Number
    // Let's create a sorted list of pairs: (LevelNumber, Name, ID)
    struct LvlInfo { int num; QString name; int id; };
    QList<LvlInfo> displayLevels;
    
    for(const auto& id : uniqueLevelIds) {
        QString name = "Year " + QString::number(id); // Fallback
        int num = id;
        
        // Find match
        for(const auto& l : allLevels) {
            if(l.id() == id) {
                name = l.name();
                num = l.levelNumber();
                break;
            }
        }
        displayLevels.append({num, name, id});
    }
    
    // Sort by Number
    std::sort(displayLevels.begin(), displayLevels.end(), [](const LvlInfo& a, const LvlInfo& b){
        return a.num < b.num;
    });
    
    for(const auto& info : displayLevels) {
        // Display as "Year X"
        m_yearSelector->addItem(QString("Year %1").arg(info.num), info.id);
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

void ProfessorPanel::onYearSelected(int index) {
    if (index < 0) return;
    int selectedLevel = m_yearSelector->currentData().toInt();
    
    m_courseSelector->blockSignals(true);
    m_courseSelector->clear();
    for(const auto& c : m_allProfessorCourses) {
        if (c.yearLevel() == selectedLevel) {
            m_courseSelector->addItem(c.name(), c.id());
        }
    }
    m_courseSelector->blockSignals(false);
    
    if (m_courseSelector->count() > 0) {
        m_courseSelector->setCurrentIndex(0);
    }
    onRefreshStudents();
}

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

void ProfessorPanel::onCourseSelected(int index) {
    onRefreshStudents();
}

void ProfessorPanel::onRefreshStudents() {
    if(m_courseSelector->count() == 0) {
        m_gradesTable->setRowCount(0);
        m_attendanceTable->setRowCount(0);
        return;
    }

    int courseId = m_courseSelector->currentData().toInt();
    QDate currentAttDate = m_attendanceDate->date();
    
    QList<Enrollment> enrollments = m_enrollmentController.getEnrollmentsByCourse(courseId);
    QList<AttendanceLog> logs = m_enrollmentController.getAttendanceLogsByCourse(courseId, currentAttDate);
    
    bool hasStudents = !enrollments.isEmpty();

    // 1. Refresh Grades Table
    if (m_gradesTable && m_gradesTable->isVisible()) {
        m_gradesTable->setRowCount(0);
        m_noStudentsLabelGrades->setVisible(!hasStudents);
        m_gradesTable->setVisible(hasStudents);

        if(hasStudents) {
            for(const auto& e : enrollments) {
                int r = m_gradesTable->rowCount();
                m_gradesTable->insertRow(r);
                
                // Code, Name, Section, Attendance, Absences, Ass1, Ass2, CW, Final, Total, Grade
                QTableWidgetItem* iCode = new QTableWidgetItem(e.studentCode());
                iCode->setData(Qt::UserRole, e.id());
                iCode->setFlags(iCode->flags() & ~Qt::ItemIsEditable);
                m_gradesTable->setItem(r, 0, iCode);
                
                QTableWidgetItem* iName = new QTableWidgetItem(e.studentName());
                iName->setFlags(iName->flags() & ~Qt::ItemIsEditable);
                m_gradesTable->setItem(r, 1, iName);
                
                QTableWidgetItem* iSec = new QTableWidgetItem(e.studentSection());
                iSec->setFlags(iSec->flags() & ~Qt::ItemIsEditable);
                m_gradesTable->setItem(r, 2, iSec);

                QTableWidgetItem* iAtt = new QTableWidgetItem(QString::number(e.attendanceCount()));
                iAtt->setFlags(iAtt->flags() & ~Qt::ItemIsEditable); 
                m_gradesTable->setItem(r, 3, iAtt);

                QTableWidgetItem* iAbs = new QTableWidgetItem(QString::number(e.absenceCount()));
                iAbs->setFlags(iAbs->flags() & ~Qt::ItemIsEditable); 
                m_gradesTable->setItem(r, 4, iAbs);

                m_gradesTable->setItem(r, 5, new QTableWidgetItem(QString::number(e.assignment1Grade())));
                m_gradesTable->setItem(r, 6, new QTableWidgetItem(QString::number(e.assignment2Grade())));
                m_gradesTable->setItem(r, 7, new QTableWidgetItem(QString::number(e.courseworkGrade())));
                m_gradesTable->setItem(r, 8, new QTableWidgetItem(QString::number(e.finalExamGrade())));
                
                QTableWidgetItem* iTot = new QTableWidgetItem(QString::number(e.totalGrade()));
                iTot->setFlags(iTot->flags() & ~Qt::ItemIsEditable);
                m_gradesTable->setItem(r, 9, iTot);

                QTableWidgetItem* iGrade = new QTableWidgetItem(e.letterGrade());
                iGrade->setFlags(iGrade->flags() & ~Qt::ItemIsEditable);
                m_gradesTable->setItem(r, 10, iGrade);
            }
        }
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
                
                QTableWidgetItem* iSec = new QTableWidgetItem(e.studentSection());
                iSec->setFlags(iSec->flags() & ~Qt::ItemIsEditable);
                m_attendanceTable->setItem(r, 2, iSec);
                
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
                
                m_attendanceTable->setCellWidget(r, 3, cb);
                m_attendanceTable->setItem(r, 4, new QTableWidgetItem("")); // Notes placeholder
            }
        }
    }
}

void ProfessorPanel::onSubmitAttendance() {
    if(m_courseSelector->count() == 0) return;
    if(!m_attendanceTable) return;

    QDate date = m_attendanceDate->date();
    
    bool allOk = true;
    for(int i=0; i<m_attendanceTable->rowCount(); ++i) {
        int eid = m_attendanceTable->item(i, 0)->data(Qt::UserRole).toInt();
        QComboBox* cb = qobject_cast<QComboBox*>(m_attendanceTable->cellWidget(i, 3)); // Col 3 is Status
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

void ProfessorPanel::onSaveGrades() {
    if(m_courseSelector->count() == 0) return;
    int cid = m_courseSelector->currentData().toInt();
    Course course = m_courseController.getCourseById(cid);
    
    bool allOk = true;
    for(int i=0; i<m_gradesTable->rowCount(); ++i) {
        Enrollment e;
        e.setId(m_gradesTable->item(i, 0)->data(Qt::UserRole).toInt());
        
        // Col indices: 0:Code, 1:Name, 2:Sec, 3:Att, 4:Abs, 5:Ass1, 6:Ass2, 7:CW, 8:Final
        
        e.setAttendanceCount(m_gradesTable->item(i, 3)->text().toInt());
        e.setAbsenceCount(m_gradesTable->item(i, 4)->text().toInt());
        
        e.setAssignment1Grade(m_gradesTable->item(i, 5)->text().toDouble());
        e.setAssignment2Grade(m_gradesTable->item(i, 6)->text().toDouble());
        e.setCourseworkGrade(m_gradesTable->item(i, 7)->text().toDouble());
        e.setFinalExamGrade(m_gradesTable->item(i, 8)->text().toDouble());
        e.setStatus("active");
        
        m_enrollmentController.calculateTotalAndGrade(e, course.courseType(), course.maxGrade());
        
        if(!m_enrollmentController.updateEnrollment(e)) {
            allOk = false;
        }
    }
    
    if(allOk) {
        QMessageBox::information(this, "Success", "Grades updated.");
        onRefreshStudents();
    } else {
        QMessageBox::warning(this, "Error", "Some records failed to update.");
    }
}


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

void ProfessorPanel::onSaveProfile() {
    m_professor.setSpecialization(m_profSpecializationEdit->text());
    m_professor.setPersonalInfo(m_profInfoEdit->toHtml());
    
    if(m_professorController.updateProfessor(m_professor)) {
        QMessageBox::information(this, "Success", "Profile updated.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update profile.");
    }
}
