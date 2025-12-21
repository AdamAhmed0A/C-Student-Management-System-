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

    // Context Tabs - Reorganized for better workflow
    m_tabWidget = new QTabWidget();
    m_tabWidget->addTab(createAttendanceTab(), "Attendance");
    m_tabWidget->addTab(createCoursesTab(), "Courses");
    m_tabWidget->addTab(createScheduleTab(), "Schedule");
    m_tabWidget->addTab(createCalendarTab(), "Calendar");
    m_tabWidget->addTab(createProfileTab(), "Profile");

    mainLayout->addWidget(m_tabWidget);
}


QWidget* ProfessorPanel::createAttendanceTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QHBoxLayout* controls = new QHBoxLayout();
    controls->addWidget(new QLabel("Select Course:"));
    m_courseSelector = new QComboBox();
    m_courseSelector->setMinimumWidth(200);
    connect(m_courseSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfessorPanel::onCourseSelected);
    controls->addWidget(m_courseSelector);
    
    controls->addSpacing(20);
    controls->addWidget(new QLabel("Select Date:"));
    m_attendanceDate = new QDateEdit(QDate::currentDate());
    m_attendanceDate->setCalendarPopup(true);
    m_attendanceDate->setMinimumWidth(120);
    connect(m_attendanceDate, &QDateEdit::dateChanged, this, &ProfessorPanel::onRefreshStudents);
    controls->addWidget(m_attendanceDate);
    
    controls->addStretch();
    
    QPushButton* refreshStdBtn = new QPushButton("Refresh List");
    connect(refreshStdBtn, &QPushButton::clicked, this, &ProfessorPanel::onRefreshStudents);
    controls->addWidget(refreshStdBtn);
    
    QPushButton* submitAttBtn = new QPushButton("Submit Attendance");
    submitAttBtn->setObjectName("primaryBtn");
    connect(submitAttBtn, &QPushButton::clicked, this, &ProfessorPanel::onSubmitAttendance);
    controls->addWidget(submitAttBtn);

    QPushButton* saveBtn = new QPushButton("Save Grades");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, this, &ProfessorPanel::onSaveGrades);
    controls->addWidget(saveBtn);
    
    layout->addLayout(controls);

    // Combined table for attendance and grades - all editable
    m_studentsTable = new QTableWidget();
    QStringList headers = {"EnrID", "Student", "Today Status", "Att Total", "Abs Total", "Ass. 1", "Ass. 2", "CW", "Final", "Total", "Grade"};
    m_studentsTable->setColumnCount(headers.size());
    m_studentsTable->setHorizontalHeaderLabels(headers);
    m_studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_studentsTable->setAlternatingRowColors(true);
    m_studentsTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    layout->addWidget(m_studentsTable);
    
    QLabel* helpText = new QLabel("Select 'Today Status' and click 'Submit Attendance'. Use 'Save Grades' for updating assignment marks.");
    helpText->setStyleSheet("color: #666; font-style: italic; padding: 5px;");
    layout->addWidget(helpText);
    
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
    m_courseSelector->blockSignals(true);
    m_courseSelector->clear();
    
    QList<Course> courses = m_courseController.getCoursesByProfessor(m_professor.id());
    
    m_coursesTable->setRowCount(0);
    
    for(const auto& c : courses) {
        m_courseSelector->addItem(c.name(), c.id());
        
        int r = m_coursesTable->rowCount();
        m_coursesTable->insertRow(r);
        m_coursesTable->setItem(r, 0, new QTableWidgetItem(QString::number(c.id())));
        m_coursesTable->setItem(r, 1, new QTableWidgetItem(c.name()));
        m_coursesTable->setItem(r, 2, new QTableWidgetItem(c.description()));
        m_coursesTable->setItem(r, 3, new QTableWidgetItem(QString::number(c.creditHours())));
        m_coursesTable->setItem(r, 4, new QTableWidgetItem(c.courseType()));
        m_coursesTable->setItem(r, 5, new QTableWidgetItem(QString::number(c.maxGrade())));
    }
    
    m_courseSelector->blockSignals(false);
    if(m_courseSelector->count() > 0) {
        m_courseSelector->setCurrentIndex(0);
        onRefreshStudents();
    }
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
    if(m_courseSelector->count() == 0) return;
    int courseId = m_courseSelector->currentData().toInt();
    QDate currentAttDate = m_attendanceDate->date();
    
    QList<Enrollment> enrollments = m_enrollmentController.getEnrollmentsByCourse(courseId);
    QList<AttendanceLog> logs = m_enrollmentController.getAttendanceLogsByCourse(courseId, currentAttDate);
    
    m_studentsTable->setRowCount(0);
    
    for(const auto& e : enrollments) {
        int r = m_studentsTable->rowCount();
        m_studentsTable->insertRow(r);
        
        m_studentsTable->setItem(r, 0, new QTableWidgetItem(QString::number(e.id())));
        QTableWidgetItem* nameItem = new QTableWidgetItem(e.studentName());
        nameItem->setFlags(nameItem->flags() & ~(Qt::ItemIsEditable)); // Remove editable flag
        m_studentsTable->setItem(r, 1, nameItem);
        
        // Today's Status Selector
        QComboBox* cb = new QComboBox();
        cb->addItems({"---", "Present", "Absent", "Late", "Excused"});
        
        // Find if log exists for today
        for(const auto& log : logs) {
            if(log.enrollmentId() == e.id()) {
                cb->setCurrentText(log.status());
                break;
            }
        }
        m_studentsTable->setCellWidget(r, 2, cb);
        
        m_studentsTable->setItem(r, 3, new QTableWidgetItem(QString::number(e.attendanceCount())));
        m_studentsTable->setItem(r, 4, new QTableWidgetItem(QString::number(e.absenceCount())));
        
        m_studentsTable->setItem(r, 5, new QTableWidgetItem(QString::number(e.assignment1Grade())));
        m_studentsTable->setItem(r, 6, new QTableWidgetItem(QString::number(e.assignment2Grade())));
        m_studentsTable->setItem(r, 7, new QTableWidgetItem(QString::number(e.courseworkGrade())));
        m_studentsTable->setItem(r, 8, new QTableWidgetItem(QString::number(e.finalExamGrade())));
        
        QTableWidgetItem* total = new QTableWidgetItem(QString::number(e.totalGrade()));
        total->setFlags(total->flags() & ~(Qt::ItemIsEditable));
        m_studentsTable->setItem(r, 9, total);
        
        QTableWidgetItem* grade = new QTableWidgetItem(e.letterGrade());
        grade->setFlags(grade->flags() & ~(Qt::ItemIsEditable));
        m_studentsTable->setItem(r, 10, grade);
    }
}

void ProfessorPanel::onSubmitAttendance() {
    if(m_courseSelector->count() == 0) return;
    QDate date = m_attendanceDate->date();
    
    bool allOk = true;
    for(int i=0; i<m_studentsTable->rowCount(); ++i) {
        int eid = m_studentsTable->item(i, 0)->text().toInt();
        QComboBox* cb = qobject_cast<QComboBox*>(m_studentsTable->cellWidget(i, 2));
        QString status = cb->currentText();
        
        if(status == "---") continue;
        
        AttendanceLog log;
        log.setEnrollmentId(eid);
        log.setDate(date);
        log.setStatus(status);
        log.setNotes("Logged via Batch Attendance");
        
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
    for(int i=0; i<m_studentsTable->rowCount(); ++i) {
        Enrollment e;
        e.setId(m_studentsTable->item(i, 0)->text().toInt());
        
        e.setAttendanceCount(m_studentsTable->item(i, 3)->text().toInt());
        e.setAbsenceCount(m_studentsTable->item(i, 4)->text().toInt());
        e.setAssignment1Grade(m_studentsTable->item(i, 5)->text().toDouble());
        e.setAssignment2Grade(m_studentsTable->item(i, 6)->text().toDouble());
        e.setCourseworkGrade(m_studentsTable->item(i, 7)->text().toDouble());
        e.setFinalExamGrade(m_studentsTable->item(i, 8)->text().toDouble());
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
