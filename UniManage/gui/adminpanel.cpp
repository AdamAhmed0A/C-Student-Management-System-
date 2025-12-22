#include "adminpanel.h"
#include "stylehelper.h"
#include "loginwindow.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTime>
#include <QTextEdit>
#include <QTabWidget>
#include <QDoubleSpinBox>
#include "../database/persistence.h"
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFont>

AdminPanel::AdminPanel(int adminId, QWidget *parent)
    : QWidget(parent), m_adminId(adminId)
{
    setStyleSheet(StyleHelper::getMainStyle());
    setupUI();
    setWindowTitle("Admin Panel - University Management System");
    resize(1280, 850);
    
    // Initial data load
    refreshAllData();
}

AdminPanel::~AdminPanel() {}

void AdminPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 30);
    mainLayout->setSpacing(20);
    
    // Header with Title and Logout
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("Administration Dashboard");
    titleLabel->setObjectName("titleLabel");
    header->addWidget(titleLabel);
    header->addStretch();
    
    QPushButton* testDbBtn = new QPushButton("Test Database");
    testDbBtn->setObjectName("primaryBtn");
    testDbBtn->setFixedWidth(150);
    connect(testDbBtn, &QPushButton::clicked, this, &AdminPanel::onTestDatabase);
    header->addWidget(testDbBtn);
    
    QPushButton* refreshBtn = new QPushButton("Refresh System");
    refreshBtn->setObjectName("secondaryBtn");
    refreshBtn->setFixedWidth(150);
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPanel::onRefreshAll);
    header->addWidget(refreshBtn);

    QPushButton* printBtn = new QPushButton("View/Print Data");
    printBtn->setObjectName("primaryBtn");
    printBtn->setFixedWidth(150);
    connect(printBtn, &QPushButton::clicked, this, &AdminPanel::onPrintData);
    header->addWidget(printBtn);

    QPushButton* logoutBtn = new QPushButton("Logout System");
    logoutBtn->setObjectName("dangerBtn");
    logoutBtn->setFixedWidth(150);
    connect(logoutBtn, &QPushButton::clicked, this, &AdminPanel::onLogout);
    header->addWidget(logoutBtn);
    
    mainLayout->addLayout(header);
    
    m_tabWidget = new QTabWidget(this);
    
    // Reorganized Tabs as requested
    m_tabWidget->addTab(createStudentsTab(), "Students");
    m_tabWidget->addTab(createFacultiesTab(), "Faculties");
    m_tabWidget->addTab(createCoursesTab(), "Academic Courses");
    m_tabWidget->addTab(createDepartmentsTab(), "Departments");
    m_tabWidget->addTab(createRoomsTab(), "Halls Labs");
    m_tabWidget->addTab(createAcademicSetupTab(), "Academic Setup");
    m_tabWidget->addTab(createProfessorsTab(), "Professor Data");
    m_tabWidget->addTab(createSchedulesTab(), "Academic Schedules");
    m_tabWidget->addTab(createSectionsTab(), "Sections/Groups");
    m_tabWidget->addTab(createCalendarTab(), "Calendar Management");
    
    mainLayout->addWidget(m_tabWidget);
}

void AdminPanel::refreshAllData() {
    refreshCollegesTable();
    refreshCoursesTable();
    refreshDepartmentsTable();
    refreshRoomsTable();
    refreshLevelsTable();
    refreshProfessorsTable();
    refreshSchedulesTable();
    refreshStudentsTable();
    refreshCalendarTable();
    refreshSectionsTable();
}

void AdminPanel::refreshSectionsTable() {
    m_sectionsTable->setRowCount(0);
    for (const auto& s : m_sectionController.getAllSections()) {
        int r = m_sectionsTable->rowCount();
        m_sectionsTable->insertRow(r);
        m_sectionsTable->setItem(r, 0, new QTableWidgetItem(QString::number(s.id())));
        m_sectionsTable->setItem(r, 1, new QTableWidgetItem(s.name()));
        m_sectionsTable->setItem(r, 2, new QTableWidgetItem(s.courseName()));
        m_sectionsTable->setItem(r, 3, new QTableWidgetItem(QString::number(s.capacity())));
        // Mapping semester id to string would be better, but we use ID for now
        m_sectionsTable->setItem(r, 4, new QTableWidgetItem(QString::number(s.semesterId())));
    }
}

void AdminPanel::refreshCalendarTable() {
    m_calendarTable->setRowCount(0);
    for (const auto& e : m_calendarController.getAllEvents()) {
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

QWidget* AdminPanel::createCalendarTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("New Event");
    QPushButton* editBtn = new QPushButton("Edit Event");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Event");
    deleteBtn->setObjectName("dangerBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();
    
    m_calendarTable = new QTableWidget();
    m_calendarTable->setColumnCount(6);
    m_calendarTable->setHorizontalHeaderLabels({"ID", "Title", "Start", "End", "Type", "Description"});
    m_calendarTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_calendarTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_calendarTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCalendarEvent);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditCalendarEvent);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteCalendarEvent);
    
    refreshCalendarTable();
    return widget;
}

void AdminPanel::onAssignProfessor() {
    int cur = m_coursesTable->currentRow();
    if (cur < 0) {
        QMessageBox::warning(this, "Selection Required", "Please select a course to assign.");
        return;
    }
    
    int courseId = m_coursesTable->item(cur, 0)->text().toInt();
    QString courseName = m_coursesTable->item(cur, 1)->text();
    
    QDialog dlg(this);
    dlg.setWindowTitle("Assign Professor to " + courseName);
    QVBoxLayout* lay = new QVBoxLayout(&dlg);
    
    lay->addWidget(new QLabel("Select Professor:"));
    QComboBox* profCb = new QComboBox();
    
    // Load professors
    QList<Professor> profs = m_professorController.getAllProfessors();
    for (const auto& p : profs) {
        profCb->addItem(p.fullName(), p.id());
    }
    lay->addWidget(profCb);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Assign");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    btns->addWidget(okBtn);
    btns->addWidget(cancelBtn);
    lay->addLayout(btns);
    
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    
    if (dlg.exec() == QDialog::Accepted) {
        int profId = profCb->currentData().toInt();
        if (m_scheduleController.assignProfessorToCourse(courseId, profId)) {
            QMessageBox::information(this, "Success", "Course assigned successfully.");
            refreshCoursesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to assign course.");
        }
    }
}

void AdminPanel::onAddCalendarEvent() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add Calendar Event");
    QFormLayout* form = new QFormLayout(&dlg);
    QLineEdit* title = new QLineEdit();
    QTextEdit* desc = new QTextEdit();
    QDateEdit* start = new QDateEdit(QDate::currentDate());
    QDateEdit* end = new QDateEdit(QDate::currentDate());
    QComboBox* type = new QComboBox();
    type->addItems({"Academic", "Holiday", "Exam", "Registration", "Other"});
    
    form->addRow("Title:", title);
    form->addRow("Description:", desc);
    form->addRow("Start:", start);
    form->addRow("End:", end);
    form->addRow("Type:", type);
    
    QPushButton* btn = new QPushButton("Add Event");
    connect(btn, &QPushButton::clicked, &dlg, &QDialog::accept);
    form->addRow(btn);
    
    if (dlg.exec() == QDialog::Accepted) {
        CalendarEvent e;
        e.setTitle(title->text());
        e.setDescription(desc->toPlainText());
        e.setStartDate(start->date());
        e.setEndDate(end->date());
        e.setEventType(type->currentText());
        if (m_calendarController.addEvent(e)) {
            QMessageBox::information(this, "Success", "Event added.");
            refreshCalendarTable();
        }
    }
}

void AdminPanel::onEditCalendarEvent() {
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
    QLineEdit* title = new QLineEdit(e.title());
    QTextEdit* desc = new QTextEdit(e.description());
    QDateEdit* start = new QDateEdit(e.startDate());
    QDateEdit* end = new QDateEdit(e.endDate());
    QComboBox* type = new QComboBox();
    type->addItems({"Academic", "Holiday", "Exam", "Registration", "Other"});
    type->setCurrentText(e.eventType());
    
    form->addRow("Title:", title);
    form->addRow("Description:", desc);
    form->addRow("Start Date:", start);
    form->addRow("End Date:", end);
    form->addRow("Event Type:", type);
    
    QPushButton* btn = new QPushButton("Save Changes");
    connect(btn, &QPushButton::clicked, &dlg, &QDialog::accept);
    form->addRow(btn);
    
    if (dlg.exec() == QDialog::Accepted) {
        e.setTitle(title->text());
        e.setDescription(desc->toPlainText());
        e.setStartDate(start->date());
        e.setEndDate(end->date());
        e.setEventType(type->currentText());
        if (m_calendarController.updateEvent(e)) {
            QMessageBox::information(this, "Success", "Event updated.");
            refreshCalendarTable();
        }
    }
}

void AdminPanel::onDeleteCalendarEvent() {
    int cur = m_calendarTable->currentRow();
    if(cur < 0) return;
    int eid = m_calendarTable->item(cur, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Delete this event?") == QMessageBox::Yes) {
        if (m_calendarController.deleteEvent(eid)) {
            refreshCalendarTable();
        }
    }
}


// Level Management
void AdminPanel::onEditLevel() {
    int row = m_levelsTable->currentRow();
    if (row < 0) return;
    int id = m_levelsTable->item(row, 0)->text().toInt();
    QString currentName = m_levelsTable->item(row, 1)->text();
    int currentNum = m_levelsTable->item(row, 2)->text().toInt();

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Academic Level");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit(currentName);
    QSpinBox* num = new QSpinBox();
    num->setRange(1, 10);
    num->setValue(currentNum);

    layout->addRow("Level Name:", name);
    layout->addRow("Level Number:", num);

    QPushButton* btn = new QPushButton("Update");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        AcademicLevel al(id, name->text(), num->value());
        if (m_academicLevelController.updateAcademicLevel(al)) {
            refreshLevelsTable();
        }
    }
}
void AdminPanel::onDeleteLevel() {
    int row = m_levelsTable->currentRow();
    if (row < 0) return;
    int id = m_levelsTable->item(row, 0)->text().toInt();
    if (m_academicLevelController.deleteAcademicLevel(id)) {
        refreshLevelsTable();
    }
}

// Section Management
void AdminPanel::onAddSection() {
    QDialog dialog(this);
    dialog.setWindowTitle("New Section/Group");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QComboBox* course = new QComboBox();
    for(const auto& c : m_courseController.getAllCourses()) course->addItem(c.name(), c.id());
    QSpinBox* cap = new QSpinBox(); cap->setRange(1, 400); cap->setValue(40);
    QComboBox* sem = new QComboBox();
    for(const auto& s : m_semesterController.getAllSemesters()) sem->addItem(QString::number(s.id()), s.id());
    
    layout->addRow("Section Name:", name);
    layout->addRow("Course:", course);
    layout->addRow("Max Capacity:", cap);
    layout->addRow("Semester ID:", sem);
    
    QPushButton* btn = new QPushButton("Create");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        Section s;
        s.setName(name->text());
        s.setCourseId(course->currentData().toInt());
        s.setCapacity(cap->value());
        s.setSemesterId(sem->currentData().toInt());
        if(m_sectionController.addSection(s)) {
            refreshSectionsTable();
        }
    }
}
void AdminPanel::onEditSection() {
    int row = m_sectionsTable->currentRow();
    if (row < 0) return;
    int id = m_sectionsTable->item(row, 0)->text().toInt();
    
    QString currentName = m_sectionsTable->item(row, 1)->text();
    QString courseName = m_sectionsTable->item(row, 2)->text();
    int currentCap = m_sectionsTable->item(row, 3)->text().toInt();
    int currentSem = m_sectionsTable->item(row, 4)->text().toInt();

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Section/Group");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit(currentName);
    QComboBox* course = new QComboBox();
    auto courses = m_courseController.getAllCourses();
    int courseIdx = -1;
    for(int i=0; i<courses.size(); ++i) {
        course->addItem(courses[i].name(), courses[i].id());
        if(courses[i].name() == courseName) courseIdx = i;
    }
    if(courseIdx != -1) course->setCurrentIndex(courseIdx);

    QSpinBox* cap = new QSpinBox(); cap->setRange(1, 400); cap->setValue(currentCap);
    QComboBox* sem = new QComboBox();
    auto semesters = m_semesterController.getAllSemesters();
    int semIdx = -1;
    for(int i=0; i<semesters.size(); ++i) {
        sem->addItem(QString::number(semesters[i].id()), semesters[i].id());
        if(semesters[i].id() == currentSem) semIdx = i;
    }
    if(semIdx != -1) sem->setCurrentIndex(semIdx);
    
    layout->addRow("Section Name:", name);
    layout->addRow("Course:", course);
    layout->addRow("Max Capacity:", cap);
    layout->addRow("Semester ID:", sem);
    
    QPushButton* btn = new QPushButton("Update");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        Section s;
        s.setId(id);
        s.setName(name->text());
        s.setCourseId(course->currentData().toInt());
        s.setCapacity(cap->value());
        s.setSemesterId(sem->currentData().toInt());
        if(m_sectionController.updateSection(s)) {
            refreshSectionsTable();
        }
    }
}
void AdminPanel::onDeleteSection() {
    int row = m_sectionsTable->currentRow();
    if (row < 0) return;
    int id = m_sectionsTable->item(row, 0)->text().toInt();
    if(m_sectionController.deleteSection(id)) refreshSectionsTable();
}

// Schedule Edits
void AdminPanel::onEditSchedule() {
    int row = m_schedulesTable->currentRow();
    if (row < 0) return;
    int id = m_schedulesTable->item(row, 0)->text().toInt();
    QString currentCourse = m_schedulesTable->item(row, 1)->text();
    QString currentRoom = m_schedulesTable->item(row, 2)->text();
    QString currentProf = m_schedulesTable->item(row, 3)->text();
    QString currentDay = m_schedulesTable->item(row, 4)->text();
    QTime currentStart = QTime::fromString(m_schedulesTable->item(row, 5)->text());
    QTime currentEnd = QTime::fromString(m_schedulesTable->item(row, 6)->text());

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Schedule Slot");
    QFormLayout* layout = new QFormLayout(&dialog);

    QComboBox* course = new QComboBox();
    auto courses = m_courseController.getAllCourses();
    for(const auto& c : courses) {
        course->addItem(c.name(), c.id());
        if(c.name() == currentCourse) course->setCurrentIndex(course->count()-1);
    }
    
    QComboBox* room = new QComboBox();
    auto rooms = m_roomController.getAllRooms();
    for(const auto& r : rooms) {
        room->addItem(r.name(), r.id());
        if(r.name() == currentRoom) room->setCurrentIndex(room->count()-1);
    }
    
    QComboBox* prof = new QComboBox();
    auto profs = m_professorController.getAllProfessors();
    for(const auto& p : profs) {
        prof->addItem(p.fullName(), p.id());
        if(p.fullName() == currentProf) prof->setCurrentIndex(prof->count()-1);
    }

    QComboBox* day = new QComboBox();
    day->addItems({"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"});
    day->setCurrentText(currentDay);

    QTimeEdit* start = new QTimeEdit(currentStart);
    QTimeEdit* end = new QTimeEdit(currentEnd);

    layout->addRow("Course:", course);
    layout->addRow("Room:", room);
    layout->addRow("Professor:", prof);
    layout->addRow("Day:", day);
    layout->addRow("Start Time:", start);
    layout->addRow("End Time:", end);

    QPushButton* btn = new QPushButton("Update");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        Schedule s;
        s.setId(id);
        s.setCourseId(course->currentData().toInt());
        s.setRoomId(room->currentData().toInt());
        s.setProfessorId(prof->currentData().toInt());
        s.setDayOfWeek(day->currentText());
        s.setStartTime(start->time());
        s.setEndTime(end->time());

        if (m_scheduleController.updateSchedule(s)) {
            refreshSchedulesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update schedule slot. Check if room is already booked.");
        }
    }
}
void AdminPanel::onDeleteSchedule() {
    int row = m_schedulesTable->currentRow();
    if (row < 0) return;
    int id = m_schedulesTable->item(row, 0)->text().toInt();
    if(m_scheduleController.deleteSchedule(id)) refreshSchedulesTable();
}

void AdminPanel::onRefreshAll() {
    refreshAllData();
    QMessageBox::information(this, "Success", "Application data has been refreshed successfully.");
}

void AdminPanel::onTestDatabase() {
    QString report = "=== DATABASE CONNECTION TEST ===\n\n";
    
    QSqlDatabase db = DBConnection::instance().database();
    
    // Test 1: Connection Status
    report += "1. CONNECTION STATUS:\n";
    if (db.isOpen()) {
        report += "   ✓ Database is CONNECTED\n";
        report += "   Database Name: " + db.databaseName() + "\n";
        report += "   Host: " + db.hostName() + "\n";
        report += "   Driver: " + db.driverName() + "\n\n";
    } else {
        report += "   ✗ Database is NOT CONNECTED\n";
        report += "   Error: " + db.lastError().text() + "\n\n";
        QMessageBox::critical(this, "Database Error", report);
        return;
    }
    
    // Test 2: Users Table
    report += "2. USERS TABLE TEST:\n";
    QSqlQuery userQuery(db);
    if (userQuery.exec("SELECT COUNT(*) as total, SUM(CASE WHEN role='student' THEN 1 ELSE 0 END) as students FROM users")) {
        if (userQuery.next()) {
            int total = userQuery.value("total").toInt();
            int students = userQuery.value("students").toInt();
            report += "   ✓ Total users: " + QString::number(total) + "\n";
            report += "   ✓ Users with role='student': " + QString::number(students) + "\n\n";
        }
    } else {
        report += "   ✗ Error: " + userQuery.lastError().text() + "\n\n";
    }
    
    // Test 3: Students Data Table
    report += "3. STUDENTS_DATA TABLE TEST:\n";
    QSqlQuery studentDataQuery(db);
    if (studentDataQuery.exec("SELECT COUNT(*) as total FROM students_data")) {
        if (studentDataQuery.next()) {
            int total = studentDataQuery.value("total").toInt();
            report += "   ✓ Total student profiles: " + QString::number(total) + "\n\n";
        }
    } else {
        report += "   ✗ Error: " + studentDataQuery.lastError().text() + "\n\n";
    }
    
    // Test 4: The Actual Student Query
    report += "4. STUDENT RETRIEVAL QUERY TEST:\n";
    report += "   Query: " + Queries::SELECT_ALL_STUDENTS_DATA + "\n\n";
    
    QSqlQuery testQuery(db);
    if (testQuery.exec(Queries::SELECT_ALL_STUDENTS_DATA)) {
        int count = 0;
        report += "   ✓ Query executed successfully!\n";
        report += "   Results:\n";
        
        while (testQuery.next() && count < 5) {
            count++;
            QString name = testQuery.value("full_name").toString();
            QString studentNum = testQuery.value("student_number").toString();
            QString role = testQuery.value("role").toString();
            int userId = testQuery.value("user_id").toInt();
            int profileId = testQuery.value("id").toInt();
            
            report += QString("   %1. %2 (Student#: %3, Role: %4, UserID: %5, ProfileID: %6)\n")
                .arg(count).arg(name).arg(studentNum).arg(role).arg(userId).arg(profileId);
        }
        
        // Count remaining
        while (testQuery.next()) count++;
        
        report += "\n   Total records returned: " + QString::number(count) + "\n\n";
        
        if (count == 0) {
            report += "   ⚠ WARNING: No students found!\n";
            report += "   This means either:\n";
            report += "   - No users have role='student' in the database\n";
            report += "   - The query has an issue\n\n";
        }
    } else {
        report += "   ✗ Query FAILED!\n";
        report += "   Error: " + testQuery.lastError().text() + "\n\n";
    }
    
    // Test 5: Check for orphaned records
    report += "5. ORPHANED RECORDS CHECK:\n";
    QSqlQuery orphanQuery(db);
    if (orphanQuery.exec("SELECT u.id, u.full_name, u.username FROM users u LEFT JOIN students_data sd ON u.id = sd.user_id WHERE u.role='student' AND sd.id IS NULL")) {
        int orphans = 0;
        QString orphanList = "";
        while (orphanQuery.next()) {
            orphans++;
            if (orphans <= 3) {
                orphanList += QString("   - %1 (ID: %2, Username: %3)\n")
                    .arg(orphanQuery.value("full_name").toString())
                    .arg(orphanQuery.value("id").toInt())
                    .arg(orphanQuery.value("username").toString());
            }
        }
        
        if (orphans > 0) {
            report += "   ⚠ Found " + QString::number(orphans) + " student users WITHOUT profile data:\n";
            report += orphanList;
            if (orphans > 3) {
                report += "   ... and " + QString::number(orphans - 3) + " more\n";
            }
            report += "\n";
        } else {
            report += "   ✓ No orphaned student records\n\n";
        }
    }
    
    report += "=== END OF TEST ===";
    
    // Show in a scrollable dialog
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Database Connection Test Results");
    dialog->resize(700, 500);
    
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QTextEdit* textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setPlainText(report);
    textEdit->setFont(QFont("Courier New", 9));
    layout->addWidget(textEdit);
    
    QPushButton* closeBtn = new QPushButton("Close");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    qDebug() << report;
    dialog->exec();
}

QWidget* AdminPanel::createFacultiesTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add Faculty");
    QPushButton* editBtn = new QPushButton("Edit Faculty");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Delete Faculty");
    deleteBtn->setObjectName("dangerBtn");
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();
    
    m_collegesTable = new QTableWidget();
    m_collegesTable->setColumnCount(4);
    m_collegesTable->setHorizontalHeaderLabels({"ID", "Faculty Name", "Code", "Tuition fees per Year"});
    m_collegesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_collegesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_collegesTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCollege);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditCollege);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteCollege);
    return widget;
}

QWidget* AdminPanel::createDepartmentsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    
    QPushButton* addBtn = new QPushButton("New Department");
    QPushButton* editBtn = new QPushButton("Edit Department");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Department");
    deleteBtn->setObjectName("dangerBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();

    m_departmentsTable = new QTableWidget();
    m_departmentsTable->setColumnCount(4);
    m_departmentsTable->setHorizontalHeaderLabels({"ID", "Dept Name", "Faculty", "Code"});
    m_departmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_departmentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_departmentsTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddDepartment);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditDepartment);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteDepartment);
    return widget;
}

QWidget* AdminPanel::createCoursesTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Create New Course");
    QPushButton* editBtn = new QPushButton("Edit Course");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Delete Course");
    deleteBtn->setObjectName("dangerBtn");
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();
    m_coursesTable = new QTableWidget();
    m_coursesTable->setColumnCount(9);
    m_coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Professor", "Type", "Max Degree", "Credits", "Level", "Semester", "Description"});
    m_coursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    QPushButton* assignBtn = new QPushButton("Assign Professor");
    assignBtn->setObjectName("primaryBtn");
    btns->addWidget(assignBtn);

    layout->addLayout(btns);
    layout->addWidget(m_coursesTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCourse);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditCourse);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteCourse);
    connect(assignBtn, &QPushButton::clicked, this, &AdminPanel::onAssignProfessor);
    
    return widget;
}

QWidget* AdminPanel::createRoomsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add Hall/Lab");
    QPushButton* editBtn = new QPushButton("Edit Facility");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Facility");
    deleteBtn->setObjectName("dangerBtn");
    QPushButton* specBtn = new QPushButton("Detailed Specs");
    specBtn->setObjectName("primaryBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addWidget(specBtn);
    btns->addStretch();
    
    m_roomsTable = new QTableWidget();
    m_roomsTable->setColumnCount(8);
    m_roomsTable->setHorizontalHeaderLabels({"ID", "Name", "Type", "Capacity", "ACs", "Fans", "Lighting", "PCs"});
    m_roomsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_roomsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_roomsTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddRoom);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditRoom);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteRoom);
    connect(specBtn, &QPushButton::clicked, this, &AdminPanel::onManageRoomSpecs);
    return widget;
}

QWidget* AdminPanel::createAcademicSetupTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    
    QPushButton* addBtn = new QPushButton("New Academic Level");
    QPushButton* editBtn = new QPushButton("Edit Level");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Level");
    deleteBtn->setObjectName("dangerBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();

    m_levelsTable = new QTableWidget();
    m_levelsTable->setColumnCount(3);
    m_levelsTable->setHorizontalHeaderLabels({"ID", "Level Name", "Level Number"});
    m_levelsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_levelsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_levelsTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddLevel);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditLevel);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteLevel);
    return widget;
}

QWidget* AdminPanel::createSectionsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    
    QPushButton* addBtn = new QPushButton("New Section/Group");
    QPushButton* editBtn = new QPushButton("Edit Section");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Section");
    deleteBtn->setObjectName("dangerBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();

    m_sectionsTable = new QTableWidget();
    m_sectionsTable->setColumnCount(5);
    m_sectionsTable->setHorizontalHeaderLabels({"ID", "Name", "Course", "Capacity", "Semester"});
    m_sectionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_sectionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_sectionsTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddSection);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditSection);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteSection);
    return widget;
}

QWidget* AdminPanel::createProfessorsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    
    QPushButton* addBtn = new QPushButton("Register Professor");
    QPushButton* editBtn = new QPushButton("Edit Professor");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Professor");
    deleteBtn->setObjectName("dangerBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();

    m_professorsTable = new QTableWidget();
    m_professorsTable->setColumnCount(5);
    m_professorsTable->setHorizontalHeaderLabels({"ID", "Name", "Code", "National ID", "Specialization"});
    m_professorsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_professorsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_professorsTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddProfessor);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditProfessor);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteProfessor);
    return widget;
}

QWidget* AdminPanel::createSchedulesTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    
    QPushButton* addBtn = new QPushButton("New Schedule Assignment");
    QPushButton* editBtn = new QPushButton("Edit Schedule");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Slot");
    deleteBtn->setObjectName("dangerBtn");
    
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();

    m_schedulesTable = new QTableWidget();
    m_schedulesTable->setColumnCount(7);
    m_schedulesTable->setHorizontalHeaderLabels({"ID", "Course", "Room", "Professor", "Day", "Start", "End"});
    m_schedulesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_schedulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(btns);
    layout->addWidget(m_schedulesTable);
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddSchedule);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditSchedule);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteSchedule);
    return widget;
}

QWidget* AdminPanel::createStudentsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Enroll New Student");
    QPushButton* editBtn = new QPushButton("Edit Details");
    editBtn->setObjectName("secondaryBtn");
    QPushButton* deleteBtn = new QPushButton("Remove Student");
    deleteBtn->setObjectName("dangerBtn");
    btns->addWidget(addBtn);
    btns->addWidget(editBtn);
    btns->addWidget(deleteBtn);
    btns->addStretch();
    m_studentsTable = new QTableWidget();
    m_studentsTable->setColumnCount(10);
    m_studentsTable->setHorizontalHeaderLabels({"ID", "Code", "Name", "ID Number", "College", "Dept", "Section", "Level", "Fees", "Status"});
    m_studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addLayout(btns);
    layout->addWidget(m_studentsTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddStudent);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteStudent);
    return widget;
}

// Slot Implementations
void AdminPanel::onLogout() {
    LoginWindow* login = new LoginWindow();
    login->show();
    this->close();
}

void AdminPanel::refreshStudentsTable() {
    qDebug() << "=== REFRESHING STUDENTS TABLE ===";
    m_studentsTable->setRowCount(0);
    
    QList<StudentData> allStudents = m_studentController.getAllStudents();
    qDebug() << "Received" << allStudents.size() << "students from controller";
    
    for (const auto& s : allStudents) {
        int r = m_studentsTable->rowCount();
        m_studentsTable->insertRow(r);
        
        // Use user ID if profile ID is missing (0)
        QString idStr = (s.id() == 0) ? QString("U-%1").arg(s.userId()) : QString::number(s.id());
        m_studentsTable->setItem(r, 0, new QTableWidgetItem(idStr));
        
        m_studentsTable->setItem(r, 1, new QTableWidgetItem(s.studentNumber()));
        m_studentsTable->setItem(r, 2, new QTableWidgetItem(s.fullName()));
        m_studentsTable->setItem(r, 3, new QTableWidgetItem(s.idNumber().isEmpty() ? "---" : s.idNumber()));
        m_studentsTable->setItem(r, 4, new QTableWidgetItem(s.collegeName().isEmpty() ? "---" : s.collegeName()));
        m_studentsTable->setItem(r, 5, new QTableWidgetItem(s.department().isEmpty() ? "---" : s.department())); // Added Department column
        m_studentsTable->setItem(r, 6, new QTableWidgetItem(s.sectionName().isEmpty() ? "---" : s.sectionName()));
        
        QString level = s.levelName();
        if(level.isEmpty()) level = (s.academicLevelId() == 0) ? "---" : QString::number(s.academicLevelId());
        m_studentsTable->setItem(r, 7, new QTableWidgetItem(level));
        
        m_studentsTable->setItem(r, 8, new QTableWidgetItem(QString("$%1").arg(s.tuitionFees())));

        QString status = s.status();
        if (status.isEmpty()) status = (s.id() == 0) ? "Incomplete Profile" : "Pending";
        m_studentsTable->setItem(r, 9, new QTableWidgetItem(status));

        // Color code incomplete records
        if (s.id() == 0) {
            for (int col = 0; col < 10; ++col) {
                m_studentsTable->item(r, col)->setForeground(Qt::red);
            }
        }
        
        if (r < 3) {
            qDebug() << "Row" << r << ":" << s.fullName() << "| Student#:" << s.studentNumber() << "| Role:" << s.role();
        }
    }
    
    qDebug() << "Students table now has" << m_studentsTable->rowCount() << "rows";
    qDebug() << "=== END REFRESH STUDENTS TABLE ===";
}

void AdminPanel::refreshCoursesTable() {
    m_coursesTable->setRowCount(0);
    m_coursesTable->setColumnCount(10);
    m_coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Department", "Professor", "Type", "Max Grade", "Credits", "Level", "Semester", "Description"});
    for (const auto& c : m_courseController.getAllCourses()) {
        int r = m_coursesTable->rowCount();
        m_coursesTable->insertRow(r);
        m_coursesTable->setItem(r, 0, new QTableWidgetItem(QString::number(c.id())));
        m_coursesTable->setItem(r, 1, new QTableWidgetItem(c.name()));
        m_coursesTable->setItem(r, 2, new QTableWidgetItem(c.departmentName().isEmpty() ? "General" : c.departmentName()));
        m_coursesTable->setItem(r, 3, new QTableWidgetItem(c.assignedProfessor().isEmpty() ? "---" : c.assignedProfessor()));
        m_coursesTable->setItem(r, 4, new QTableWidgetItem(c.courseType()));
        m_coursesTable->setItem(r, 5, new QTableWidgetItem(QString::number(c.maxGrade())));
        m_coursesTable->setItem(r, 6, new QTableWidgetItem(QString::number(c.creditHours())));
        m_coursesTable->setItem(r, 7, new QTableWidgetItem(QString::number(c.yearLevel())));
        m_coursesTable->setItem(r, 8, new QTableWidgetItem(c.semesterName()));
        m_coursesTable->setItem(r, 9, new QTableWidgetItem(c.description()));
    }
}

void AdminPanel::refreshCollegesTable() {
    m_collegesTable->setRowCount(0);
    for (const auto& c : m_collegeController.getAllColleges()) {
        int r = m_collegesTable->rowCount();
        m_collegesTable->insertRow(r);
        m_collegesTable->setItem(r, 0, new QTableWidgetItem(QString::number(c.id())));
        m_collegesTable->setItem(r, 1, new QTableWidgetItem(c.name()));
        m_collegesTable->setItem(r, 2, new QTableWidgetItem(c.code()));
        m_collegesTable->setItem(r, 3, new QTableWidgetItem(QString("$%1").arg(c.tuitionFees())));
    }
}

void AdminPanel::refreshDepartmentsTable() {
    m_departmentsTable->setRowCount(0);
    for (const auto& d : m_departmentController.getAllDepartments()) {
        int r = m_departmentsTable->rowCount();
        m_departmentsTable->insertRow(r);
        m_departmentsTable->setItem(r, 0, new QTableWidgetItem(QString::number(d.id())));
        m_departmentsTable->setItem(r, 1, new QTableWidgetItem(d.name()));
        m_departmentsTable->setItem(r, 2, new QTableWidgetItem(d.collegeName()));
        m_departmentsTable->setItem(r, 3, new QTableWidgetItem(d.code()));
    }
}

void AdminPanel::refreshLevelsTable() {
    m_levelsTable->setRowCount(0);
    for (const auto& l : m_academicLevelController.getAllAcademicLevels()) {
        int r = m_levelsTable->rowCount();
        m_levelsTable->insertRow(r);
        m_levelsTable->setItem(r, 0, new QTableWidgetItem(QString::number(l.id())));
        m_levelsTable->setItem(r, 1, new QTableWidgetItem(l.name()));
        m_levelsTable->setItem(r, 2, new QTableWidgetItem(QString::number(l.levelNumber())));
    }
}

void AdminPanel::refreshRoomsTable() {
    m_roomsTable->setRowCount(0);
    for (const auto& r_obj : m_roomController.getAllRooms()) {
        int r = m_roomsTable->rowCount();
        m_roomsTable->insertRow(r);
        m_roomsTable->setItem(r, 0, new QTableWidgetItem(QString::number(r_obj.id())));
        m_roomsTable->setItem(r, 1, new QTableWidgetItem(r_obj.name()));
        m_roomsTable->setItem(r, 2, new QTableWidgetItem(r_obj.type()));
        m_roomsTable->setItem(r, 3, new QTableWidgetItem(QString::number(r_obj.capacity())));
        m_roomsTable->setItem(r, 4, new QTableWidgetItem(QString::number(r_obj.acUnits())));
        m_roomsTable->setItem(r, 5, new QTableWidgetItem(QString::number(r_obj.fansCount())));
        m_roomsTable->setItem(r, 6, new QTableWidgetItem(QString::number(r_obj.lightingPoints())));
        m_roomsTable->setItem(r, 7, new QTableWidgetItem(QString::number(r_obj.computersCount())));
    }
}

void AdminPanel::refreshProfessorsTable() {
    m_professorsTable->setRowCount(0);
    for (const auto& p : m_professorController.getAllProfessors()) {
        int r = m_professorsTable->rowCount();
        m_professorsTable->insertRow(r);
        m_professorsTable->setItem(r, 0, new QTableWidgetItem(QString::number(p.id())));
        m_professorsTable->setItem(r, 1, new QTableWidgetItem(p.fullName()));
        
        // Get username (code) from user table
        User u = m_userController.getUserById(p.userId());
        m_professorsTable->setItem(r, 2, new QTableWidgetItem(u.username()));
        
        m_professorsTable->setItem(r, 3, new QTableWidgetItem(p.idNumber()));
        m_professorsTable->setItem(r, 4, new QTableWidgetItem(p.specialization()));
    }
}

void AdminPanel::refreshSchedulesTable() {
    m_schedulesTable->setRowCount(0);
    QSqlQuery query("SELECT s.id, c.name, r.name, u.full_name, s.day_of_week, s.start_time, s.end_time "
                    "FROM schedules s "
                    "JOIN courses c ON s.course_id = c.id "
                    "JOIN rooms r ON s.room_id = r.id "
                    "JOIN professors p ON s.professor_id = p.id "
                    "JOIN users u ON p.user_id = u.id", 
                    DBConnection::instance().database());
    
    if (query.exec()) {
        while (query.next()) {
            int r = m_schedulesTable->rowCount();
            m_schedulesTable->insertRow(r);
            for(int i=0; i<7; ++i) {
                m_schedulesTable->setItem(r, i, new QTableWidgetItem(query.value(i).toString()));
            }
        }
    }
}

void AdminPanel::onAddStudent() {
    QDialog dialog(this);
    dialog.setWindowTitle("Student Registration");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit();
    QLineEdit* nationalId = new QLineEdit();
    nationalId->setMaxLength(14);
    nationalId->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    
    QLineEdit* code = new QLineEdit();
    
    QComboBox* college = new QComboBox();
    QList<College> colleges = m_collegeController.getAllColleges();
    for(const auto& c : colleges) college->addItem(c.name(), c.id());
    
    QComboBox* dept = new QComboBox();
    auto updateDepts = [this, college, dept, colleges]() {
        dept->clear();
        int cid = college->currentData().toInt();
        for(const auto& d : m_departmentController.getAllDepartments()) {
            if(d.collegeId() == cid) dept->addItem(d.name(), d.id());
        }
    };
    connect(college, &QComboBox::currentIndexChanged, updateDepts);
    updateDepts();
    
    QComboBox* level = new QComboBox();
    for(const auto& l : m_academicLevelController.getAllAcademicLevels()) level->addItem(l.name(), l.id());

    QComboBox* section = new QComboBox();
    section->addItem("--- No Section ---", 0);
    auto updateSections = [this, section]() {
        section->clear();
        section->addItem("--- No Section ---", 0);
        for(const auto& s : m_sectionController.getAllSections()) {
            section->addItem(s.name() + " (" + s.courseName() + ")", s.id());
        }
    };
    updateSections();

    QDoubleSpinBox* tuition = new QDoubleSpinBox();
    tuition->setRange(0, 1000000);
    tuition->setPrefix("$");
    
    auto updateTuition = [college, tuition, colleges]() {
        int cid = college->currentData().toInt();
        for(const auto& c : colleges) {
            if(c.id() == cid) {
                tuition->setValue(c.tuitionFees());
                break;
            }
        }
    };
    connect(college, &QComboBox::currentIndexChanged, updateTuition);
    updateTuition();

    layout->addRow("Full Name:", name);
    layout->addRow("National ID:", nationalId);
    layout->addRow("Student Code:", code);
    layout->addRow("College/Faculty:", college);
    layout->addRow("Department:", dept);
    layout->addRow("Academic Level:", level);
    layout->addRow("Section/Group:", section);
    layout->addRow("Assigned Tuition Fees:", tuition);

    QPushButton* btn = new QPushButton("Register Student");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty() || code->text().isEmpty() || nationalId->text().length() != 14) {
            QMessageBox::warning(this, "Input Error", "Please fill all fields. National ID must be 14 digits.");
            return;
        }

        User u; 
        u.setFullName(name->text()); 
        u.setUsername(code->text()); 
        u.setRole("student"); 
        u.setPassword(code->text()); // Initial password is their code

        QString userError;
        bool userReady = false;
        User cu;

        if (m_userController.addUser(u, &userError)) {
            cu = m_userController.getUserByUsername(code->text());
            userReady = (cu.id() > 0);
        } else if (userError.contains("Duplicate entry", Qt::CaseInsensitive)) {
            // Check if this existing user is a "ghost" (no student data)
            cu = m_userController.getUserByUsername(code->text());
            if (cu.id() > 0) {
                StudentData existing = m_studentController.getStudentByUserId(cu.id());
                if (existing.id() == 0) {
                    // Profile is missing, we can "heal" this by proceeding
                    userReady = true;
                } else {
                    QMessageBox::warning(this, "Duplicate Student", "The Student Code '" + code->text() + "' is already fully registered in the system.");
                    return;
                }
            }
        }

        if (userReady) {
            StudentData sd; 
            sd.setUserId(cu.id()); 
            sd.setStudentNumber(code->text()); 
            sd.setIdNumber(nationalId->text()); 
            sd.setDepartmentId(dept->currentData().toInt());
            sd.setAcademicLevelId(level->currentData().toInt());
            sd.setSectionId(section->currentData().toInt());
            sd.setCollegeId(college->currentData().toInt());
            sd.setTuitionFees(tuition->value());
            sd.setStatus("active");

            if (m_studentController.addStudent(sd)) {
                QMessageBox::information(this, "Success", "Student registered successfully (Linked to existing identity).");
                refreshStudentsTable();
            } else {
                // Only rollback if we just created the user in this session
                if (!userError.contains("Duplicate entry")) {
                    m_userController.deleteUser(cu.id());
                }
                QMessageBox::critical(this, "Database Error", "Failed to save student profile details.");
            }
        } else {
            QMessageBox::critical(this, "Identity Error", "Failed to setup user account.\n\nDetails: " + userError);
        }
    }
}

void AdminPanel::onEditStudent() {
    int row = m_studentsTable->currentRow();
    if (row < 0) return;
    int studentId = m_studentsTable->item(row, 0)->text().toInt();
    StudentData student = m_studentController.getStudentById(studentId);
    if (student.id() == 0) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Student Profile");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* nameEdit = new QLineEdit(student.fullName());
    QLineEdit* idEdit = new QLineEdit(student.idNumber());
    idEdit->setMaxLength(14);
    idEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    
    QComboBox* collegeEdit = new QComboBox();
    QList<College> colleges = m_collegeController.getAllColleges();
    for(const auto& c : colleges) collegeEdit->addItem(c.name(), c.id());
    collegeEdit->setCurrentIndex(collegeEdit->findData(student.collegeId()));

    QComboBox* deptEdit = new QComboBox();
    auto updateDepts = [this, collegeEdit, deptEdit]() {
        deptEdit->clear();
        int cid = collegeEdit->currentData().toInt();
        for(const auto& d : m_departmentController.getAllDepartments()) {
            if(d.collegeId() == cid) deptEdit->addItem(d.name(), d.id());
        }
    };
    connect(collegeEdit, &QComboBox::currentIndexChanged, updateDepts);
    updateDepts();
    deptEdit->setCurrentIndex(deptEdit->findData(student.departmentId()));

    QComboBox* levelEdit = new QComboBox();
    for(const auto& l : m_academicLevelController.getAllAcademicLevels()) levelEdit->addItem(l.name(), l.id());
    levelEdit->setCurrentIndex(levelEdit->findData(student.academicLevelId()));

    QComboBox* sectionEdit = new QComboBox();
    sectionEdit->addItem("--- No Section ---", 0);
    for(const auto& sect : m_sectionController.getAllSections()) {
        sectionEdit->addItem(sect.name() + " (" + sect.courseName() + ")", sect.id());
    }
    sectionEdit->setCurrentIndex(sectionEdit->findData(student.sectionId()));

    QDoubleSpinBox* tuitionEdit = new QDoubleSpinBox();
    tuitionEdit->setRange(0, 1000000);
    tuitionEdit->setPrefix("$");
    tuitionEdit->setValue(student.tuitionFees());

    layout->addRow("Full Name:", nameEdit);
    layout->addRow("National ID:", idEdit);
    layout->addRow("College/Faculty:", collegeEdit);
    layout->addRow("Department:", deptEdit);
    layout->addRow("Academic Level:", levelEdit);
    layout->addRow("Section/Group:", sectionEdit);
    layout->addRow("Tuition Fees:", tuitionEdit);

    QPushButton* ok = new QPushButton("Save Changes");
    layout->addRow(ok);
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (idEdit->text().length() != 14) {
            QMessageBox::warning(this, "Validation Error", "National ID must be exactly 14 digits.");
            return;
        }
        student.setFullName(nameEdit->text());
        student.setIdNumber(idEdit->text());
        student.setCollegeId(collegeEdit->currentData().toInt());
        student.setDepartmentId(deptEdit->currentData().toInt());
        student.setAcademicLevelId(levelEdit->currentData().toInt());
        student.setSectionId(sectionEdit->currentData().toInt());
        student.setTuitionFees(tuitionEdit->value());

        if (m_studentController.updateStudent(student)) {
            QMessageBox::information(this, "Success", "Student records updated successfully.");
            refreshStudentsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update student records.");
        }
    }
}

void AdminPanel::onDeleteStudent() {
    int row = m_studentsTable->currentRow();
    if (row < 0) return;
    int id = m_studentsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Are you sure you want to delete this student?") == QMessageBox::Yes) {
        if (m_studentController.deleteStudent(id)) {
            QMessageBox::information(this, "Deleted", "Student has been removed from the system.");
            refreshStudentsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete student.");
        }
    }
}

void AdminPanel::onAddCourse() {
    QDialog dialog(this);
    dialog.setWindowTitle("New Academic Course");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit();
    QComboBox* type = new QComboBox(); type->addItems({"Theoretical", "Practical"});
    QComboBox* maxG = new QComboBox(); maxG->addItems({"100", "150"});
    QSpinBox* credits = new QSpinBox(); credits->setRange(1, 10);
    
    QComboBox* level = new QComboBox();
    for(const auto& l : m_academicLevelController.getAllAcademicLevels()) level->addItem(l.name(), l.id());
    
    QComboBox* semester = new QComboBox();
    for(const auto& s : m_semesterController.getAllSemesters()) {
        semester->addItem(QString("Year %1 - Sem %2").arg(QString::number(s.year().date().year())).arg(QString::number(s.semester())), s.id());
    }

    QComboBox* dept = new QComboBox();
    for(const auto& d : m_departmentController.getAllDepartments()) dept->addItem(d.name(), d.id());

    QComboBox* prof = new QComboBox();
    prof->addItem("None (Assign later)", 0);
    for(const auto& p : m_professorController.getAllProfessors()) prof->addItem(p.fullName(), p.id());

    layout->addRow("Course Name:", name);
    layout->addRow("Department (Mandatory):", dept);
    layout->addRow("Type:", type);
    layout->addRow("Max Points:", maxG);
    layout->addRow("Credit Hours:", credits);
    layout->addRow("Academic Level:", level);
    layout->addRow("Semester:", semester);
    layout->addRow("Assign Professor:", prof);

    QPushButton* btn = new QPushButton("Create Course");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Course name cannot be empty.");
            return;
        }
        Course c; 
        c.setName(name->text()); 
        c.setCourseType(type->currentText()); 
        c.setMaxGrade(maxG->currentText().toInt()); 
        c.setCreditHours(credits->value());
        c.setYearLevel(level->currentData().toInt());
        c.setSemesterId(semester->currentData().toInt());
        c.setDepartmentId(dept->currentData().toInt());

        if (c.departmentId() <= 0) {
            QMessageBox::warning(this, "Constraint Error", "A Course MUST be linked to a Department.");
            return;
        }

        if (m_courseController.addCourse(c)) {
            if (prof->currentData().toInt() > 0) {
                QSqlQuery idQ(DBConnection::instance().database());
                idQ.exec("SELECT LAST_INSERT_ID()");
                int nid = 0;
                if (idQ.next()) {
                    nid = idQ.value(0).toInt();
                }
                m_scheduleController.assignProfessorToCourse(nid, prof->currentData().toInt());
            }
            QMessageBox::information(this, "Success", "Course created successfully.");
            refreshCoursesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to create course. Please check database connectivity.");
        }
    }
}

void AdminPanel::onEditCourse() {
    int row = m_coursesTable->currentRow();
    if (row < 0) return;
    int id = m_coursesTable->item(row, 0)->text().toInt();
    Course c = m_courseController.getCourseById(id);
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Course Details");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit(c.name());
    QComboBox* type = new QComboBox(); type->addItems({"Theoretical", "Practical"});
    type->setCurrentText(c.courseType());
    
    QComboBox* maxG = new QComboBox(); maxG->addItems({"100", "150"});
    maxG->setCurrentText(QString::number(c.maxGrade()));
    
    QSpinBox* credits = new QSpinBox(); credits->setRange(1, 10);
    credits->setValue(c.creditHours());

    QComboBox* level = new QComboBox();
    for(const auto& l : m_academicLevelController.getAllAcademicLevels()) {
        level->addItem(l.name(), l.id());
        if (l.id() == c.yearLevel()) level->setCurrentIndex(level->count()-1);
    }

    QComboBox* semester = new QComboBox();
    for(const auto& s : m_semesterController.getAllSemesters()) {
        semester->addItem(QString("Year %1 - Sem %2").arg(QString::number(s.year().date().year())).arg(QString::number(s.semester())), s.id());
        if (s.id() == c.semesterId()) semester->setCurrentIndex(semester->count()-1);
    }
    
    QComboBox* dept = new QComboBox();
    for(const auto& d : m_departmentController.getAllDepartments()) {
        dept->addItem(d.name(), d.id());
        if (d.id() == c.departmentId()) dept->setCurrentIndex(dept->count()-1);
    }

    QComboBox* prof = new QComboBox();
    prof->addItem("Unchanged", -1);
    prof->addItem("None", 0);
    for(const auto& p : m_professorController.getAllProfessors()) prof->addItem(p.fullName(), p.id());

    layout->addRow("Course Name:", name);
    layout->addRow("Department (Mandatory):", dept);
    layout->addRow("Type:", type);
    layout->addRow("Max Grade:", maxG);
    layout->addRow("Credits:", credits);
    layout->addRow("Academic Level:", level);
    layout->addRow("Semester:", semester);
    layout->addRow("Assign Professor:", prof);

    QPushButton* ok = new QPushButton("Save Changes");
    layout->addRow(ok);
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        c.setName(name->text());
        c.setCourseType(type->currentText());
        c.setMaxGrade(maxG->currentText().toInt());
        c.setCreditHours(credits->value());
        c.setYearLevel(level->currentData().toInt());
        c.setSemesterId(semester->currentData().toInt());
        c.setDepartmentId(dept->currentData().toInt());

        if (c.departmentId() <= 0) {
            QMessageBox::warning(this, "Constraint Error", "A Course MUST be linked to a Department.");
            return;
        }

        if (m_courseController.updateCourse(c)) {
            if (prof->currentData().toInt() >= 0) {
                m_scheduleController.assignProfessorToCourse(c.id(), prof->currentData().toInt());
            }
            QMessageBox::information(this, "Success", "Course updated successfully.");
            refreshCoursesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update course.");
        }
    }
}

void AdminPanel::onDeleteCourse() {
    int row = m_coursesTable->currentRow();
    if (row < 0) return;
    int id = m_coursesTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Delete this course?") == QMessageBox::Yes) {
        if (m_courseController.deleteCourse(id)) {
            QMessageBox::information(this, "Deleted", "Course removed.");
            refreshCoursesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete course.");
        }
    }
}

void AdminPanel::onAddRoom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Hall/Lab");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit();
    QLineEdit* code = new QLineEdit();
    QComboBox* type = new QComboBox(); type->addItems({"Hall", "Lab"});
    QSpinBox* cap = new QSpinBox(); cap->setRange(1, 1000);
    
    QSpinBox* ac = new QSpinBox(); ac->setRange(0, 50);
    QSpinBox* fans = new QSpinBox(); fans->setRange(0, 50);
    QSpinBox* lights = new QSpinBox(); lights->setRange(0, 200);
    QSpinBox* pcs = new QSpinBox(); pcs->setRange(0, 100);
    
    QTextEdit* desc = new QTextEdit();
    desc->setMaximumHeight(60);

    layout->addRow("Room Name:", name);
    layout->addRow("Room Code (Unique):", code);
    layout->addRow("Type:", type);
    layout->addRow("Capacity:", cap);
    layout->addRow("AC Units:", ac);
    layout->addRow("Fans:", fans);
    layout->addRow("Lighting Points:", lights);
    
    QLabel* pcLabel = new QLabel("Computers (for Labs):");
    layout->addRow(pcLabel, pcs);
    QLabel* seatLabel = new QLabel("Seating Description:");
    layout->addRow(seatLabel, desc);

    auto updateVisibility = [type, pcs, pcLabel, desc, seatLabel]() {
        bool isLab = (type->currentText() == "Lab");
        pcs->setVisible(isLab);
        pcLabel->setVisible(isLab);
        desc->setVisible(!isLab);
        seatLabel->setVisible(!isLab);
    };
    connect(type, &QComboBox::currentIndexChanged, updateVisibility);
    updateVisibility();

    QPushButton* btn = new QPushButton("Save Room");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty() || code->text().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Room name and code are required.");
            return;
        }
        Room r; 
        r.setName(name->text()); 
        r.setCode(code->text());
        r.setType(type->currentText()); 
        r.setCapacity(cap->value());
        r.setAcUnits(ac->value());
        r.setFansCount(fans->value());
        r.setLightingPoints(lights->value());
        r.setComputersCount(pcs->value());
        r.setSeatingDescription(desc->toPlainText());

        if (m_roomController.addRoom(r)) {
            QMessageBox::information(this, "Success", "Physical room/lab registered.");
            refreshRoomsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add room to registry.");
        }
    }
}

void AdminPanel::onEditRoom() {
    int row = m_roomsTable->currentRow();
    if (row < 0) return;
    int id = m_roomsTable->item(row, 0)->text().toInt();
    Room r = m_roomController.getRoomById(id);
    if (r.id() == 0) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Faculty Facility");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit(r.name());
    QComboBox* type = new QComboBox(); type->addItems({"Hall", "Lab"});
    type->setCurrentText(r.type());
    QSpinBox* cap = new QSpinBox(); cap->setRange(1, 1000); cap->setValue(r.capacity());
    QSpinBox* ac = new QSpinBox(); ac->setRange(0, 50); ac->setValue(r.acUnits());
    QSpinBox* pcs = new QSpinBox(); pcs->setRange(0, 100); pcs->setValue(r.computersCount());
    QTextEdit* desc = new QTextEdit(r.seatingDescription());
    desc->setMaximumHeight(60);

    layout->addRow("Name:", name);
    layout->addRow("Type:", type);
    layout->addRow("Capacity:", cap);
    layout->addRow("AC Units:", ac);
    
    QLabel* pcLabel = new QLabel("Computers:");
    layout->addRow(pcLabel, pcs);
    QLabel* seatLabel = new QLabel("Seating:");
    layout->addRow(seatLabel, desc);

    auto updateVis = [type, pcs, pcLabel, desc, seatLabel]() {
        bool isLab = (type->currentText() == "Lab");
        pcs->setVisible(isLab);
        pcLabel->setVisible(isLab);
        desc->setVisible(!isLab);
        seatLabel->setVisible(!isLab);
    };
    connect(type, &QComboBox::currentIndexChanged, updateVis);
    updateVis();

    QPushButton* btn = new QPushButton("Update Facility");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        r.setName(name->text());
        r.setType(type->currentText());
        r.setCapacity(cap->value());
        r.setAcUnits(ac->value());
        r.setComputersCount(type->currentText() == "Lab" ? pcs->value() : 0);
        r.setSeatingDescription(type->currentText() == "Lab" ? "" : desc->toPlainText());

        if (m_roomController.updateRoom(r)) {
            QMessageBox::information(this, "Success", "Facility details updated.");
            refreshRoomsTable();
        }
    }
}

void AdminPanel::onDeleteRoom() {
    int row = m_roomsTable->currentRow();
    if (row < 0) return;
    int id = m_roomsTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "Confirm Removal", "Are you sure you want to remove this facility?") == QMessageBox::Yes) {
        if (m_roomController.deleteRoom(id)) {
            refreshRoomsTable();
        }
    }
}

void AdminPanel::onAddCollege() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add College");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QLineEdit* code = new QLineEdit();
    QDoubleSpinBox* fees = new QDoubleSpinBox();
    fees->setRange(0, 1000000);
    fees->setPrefix("$");
    
    layout->addRow("College Name:", name);
    layout->addRow("Code:", code);
    layout->addRow("Yearly Tuition Fees:", fees);
    QPushButton* btn = new QPushButton("Add");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty() || code->text().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Name and Code are both required.");
            return;
        }
        College c; c.setName(name->text()); c.setCode(code->text());
        c.setTuitionFees(fees->value());
        if (m_collegeController.addCollege(c)) {
            QMessageBox::information(this, "Success", "College/Faculty added.");
            refreshCollegesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add college.");
        }
    }
}

void AdminPanel::onEditCollege() {
    int row = m_collegesTable->currentRow();
    if (row < 0) return;
    int id = m_collegesTable->item(row, 0)->text().toInt();
    
    QList<College> all = m_collegeController.getAllColleges();
    College target;
    for(const auto& c : all) if(c.id() == id) { target = c; break; }
    if(target.id() == 0) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Faculty Info");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit(target.name());
    QLineEdit* code = new QLineEdit(target.code());
    QDoubleSpinBox* fees = new QDoubleSpinBox();
    fees->setRange(0, 1000000);
    fees->setValue(target.tuitionFees());
    
    layout->addRow("Name:", name);
    layout->addRow("Code:", code);
    layout->addRow("Tuition Fees:", fees);
    
    QPushButton* btn = new QPushButton("Save");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        target.setName(name->text());
        target.setCode(code->text());
        target.setTuitionFees(fees->value());
        if (m_collegeController.updateCollege(target)) {
            refreshCollegesTable();
        }
    }
}

void AdminPanel::onDeleteCollege() {
    int row = m_collegesTable->currentRow();
    if (row < 0) return;
    int id = m_collegesTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "Confirm", "Deleting a Faculty will NOT remove its departments automatically. Proceed?") == QMessageBox::Yes) {
        if (m_collegeController.deleteCollege(id)) {
            refreshCollegesTable();
        }
    }
}

void AdminPanel::onPrintData() {
    QString content = "=== UNIMANAGE SYSTEM DATA EXPORT ===\n";
    content += "Generated on: " + QDateTime::currentDateTime().toString() + "\n\n";

    content += "--- COLLEGES/FACULTIES ---\n";
    for(const auto& c : m_collegeController.getAllColleges()) {
        content += QString("ID: %1 | Name: %2 | Code: %3 | Tuition: $%4\n")
                   .arg(c.id()).arg(c.name()).arg(c.code()).arg(c.tuitionFees());
    }

    content += "\n--- COURSES ---\n";
    for(const auto& crs : m_courseController.getAllCourses()) {
        content += QString("ID: %1 | Name: %2 | Dept: %3 | Prof: %4\n")
                   .arg(crs.id()).arg(crs.name()).arg(crs.departmentName()).arg(crs.assignedProfessor());
    }

    content += "\n--- STUDENTS ---\n";
    for(const auto& s : m_studentController.getAllStudents()) {
        content += QString("Num: %1 | Name: %2 | College: %3 | Tuition Paid/Fees: %4\n")
                   .arg(s.studentNumber()).arg(s.fullName()).arg(s.collegeName()).arg(s.tuitionFees());
    }

    if (Persistence::exportData("unimanage_full_report.txt", content)) {
        QMessageBox::information(this, "Export Success", "Full system report saved to D:/unimanage_full_report.txt");
    } else {
        QMessageBox::critical(this, "Export Failed", "Could not write to D:/. Please check permissions.");
    }
}

void AdminPanel::onEditDepartment() {
    int row = m_departmentsTable->currentRow();
    if (row < 0) return;
    int id = m_departmentsTable->item(row, 0)->text().toInt();
    Department d = m_departmentController.getDepartmentById(id);
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Department");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit(d.name());
    QLineEdit* code = new QLineEdit(d.code());
    QComboBox* faculty = new QComboBox();
    for(const auto& c : m_collegeController.getAllColleges()) faculty->addItem(c.name(), c.id());
    faculty->setCurrentIndex(faculty->findData(d.collegeId()));
    
    layout->addRow("Name:", name);
    layout->addRow("Code:", code);
    layout->addRow("Faculty:", faculty);
    
    QPushButton* btn = new QPushButton("Save");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    
    if (dialog.exec() == QDialog::Accepted) {
        d.setName(name->text());
        d.setCode(code->text());
        d.setCollegeId(faculty->currentData().toInt());
        if (m_departmentController.updateDepartment(d)) refreshDepartmentsTable();
    }
}

void AdminPanel::onDeleteDepartment() {
    int row = m_departmentsTable->currentRow();
    if (row < 0) return;
    int id = m_departmentsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Remove this department?") == QMessageBox::Yes) {
        if (m_departmentController.deleteDepartment(id)) refreshDepartmentsTable();
        else QMessageBox::warning(this, "Constraint", "Cannot delete department with active linked courses.");
    }
}

void AdminPanel::onAddDepartment() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Department");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QComboBox* college = new QComboBox();
    for(const auto& c : m_collegeController.getAllColleges()) college->addItem(c.name(), c.id());
    QLineEdit* code = new QLineEdit();
    layout->addRow("Name:", name);
    layout->addRow("College:", college);
    layout->addRow("Code:", code);
    QPushButton* btn = new QPushButton("Add");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty() || college->currentData().toInt() == 0) {
            QMessageBox::warning(this, "Input Error", "Please provide a name and select a parent College.");
            return;
        }
        Department d; d.setName(name->text()); d.setCollegeId(college->currentData().toInt()); d.setCode(code->text());
        if (m_departmentController.addDepartment(d)) {
            QMessageBox::information(this, "Success", "Department created.");
            refreshDepartmentsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to create department.");
        }
    }
}

void AdminPanel::onAddLevel() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Level");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QSpinBox* num = new QSpinBox(); num->setRange(1, 10);
    layout->addRow("Name:", name);
    layout->addRow("Number:", num);
    QPushButton* btn = new QPushButton("Add");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Level name cannot be empty.");
            return;
        }
        AcademicLevel l; l.setName(name->text()); l.setLevelNumber(num->value());
        if (m_academicLevelController.addAcademicLevel(l)) {
            QMessageBox::information(this, "Success", "Academic level added.");
            refreshLevelsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add level.");
        }
    }
}

void AdminPanel::onAddProfessor() {
    QDialog dialog(this);
    dialog.setWindowTitle("Register Professor");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit();
    QLineEdit* profCode = new QLineEdit();
    profCode->setPlaceholderText("Enter unique professor code (e.g., PROF001)");
    
    QLineEdit* idNum = new QLineEdit();
    idNum->setMaxLength(14);
    idNum->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    
    QLineEdit* spec = new QLineEdit();

    layout->addRow("Full Name:", name);
    layout->addRow("Professor Code:", profCode);
    layout->addRow("National ID:", idNum);
    layout->addRow("Specialization:", spec);
    
    QLabel* noteLabel = new QLabel("Note: The Professor Code will be used as the login password.");
    noteLabel->setStyleSheet("color: #666; font-style: italic; font-size: 11px;");
    layout->addRow(noteLabel);

    QPushButton* btn = new QPushButton("Register Professor");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (name->text().isEmpty() || profCode->text().isEmpty() || idNum->text().length() != 14) {
            QMessageBox::warning(this, "Input Error", "Name, Professor Code, and 14-digit National ID are required.");
            return;
        }

        User u; 
        u.setFullName(name->text()); 
        u.setUsername(profCode->text()); 
        u.setRole("professor"); 
        u.setPassword(profCode->text()); // Password is the professor code

        QString userError;
        bool userReady = false;
        User cu;

        if (m_userController.addUser(u, &userError)) {
            cu = m_userController.getUserByUsername(profCode->text());
            userReady = (cu.id() > 0);
        } else if (userError.contains("Duplicate entry", Qt::CaseInsensitive)) {
            // Check if this existing user is a "ghost" (no professor data)
            cu = m_userController.getUserByUsername(profCode->text());
            if (cu.id() > 0) {
                Professor existing = m_professorController.getProfessorByUserId(cu.id());
                if (existing.id() == 0) {
                    // Faculty Profile is missing, link to existing account
                    userReady = true;
                } else {
                    QMessageBox::warning(this, "Duplicate Professor", "This Professor Code is already registered.");
                    return;
                }
            }
        }

        if (userReady) {
            Professor p; 
            p.setUserId(cu.id()); 
            p.setIdNumber(idNum->text()); 
            p.setSpecialization(spec->text());

            if (m_professorController.addProfessor(p)) {
                QString successMsg = QString("Professor registered successfully!\n\n"
                                            "Login Credentials:\n"
                                            "Username: %1\n"
                                            "Password: %2\n\n"
                                            "Please provide these credentials to the professor.")
                                            .arg(profCode->text())
                                            .arg(profCode->text());
                QMessageBox::information(this, "Success", successMsg);
                refreshProfessorsTable();
            } else {
                // Only rollback if we just created the user in this session
                if (!userError.contains("Duplicate entry")) {
                    m_userController.deleteUser(cu.id());
                }
                QMessageBox::critical(this, "Error", "Failed to add professor data details.");
            }
        } else {
            QMessageBox::critical(this, "Identity Error", "Failed to setup faculty account.\n\nDetails: " + userError);
        }
    }
}

void AdminPanel::onEditProfessor() {
    int row = m_professorsTable->currentRow();
    if (row < 0) return;
    int profId = m_professorsTable->item(row, 0)->text().toInt();
    Professor p = m_professorController.getProfessorById(profId);
    if (p.id() == 0) return;

    // Get current user to retrieve username (professor code)
    User currentUser = m_userController.getUserById(p.userId());
    QString currentCode = currentUser.username();

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Professor Details");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* name = new QLineEdit(p.fullName());
    QLineEdit* profCode = new QLineEdit(currentCode);
    profCode->setPlaceholderText("Professor login code");
    
    QLineEdit* idNum = new QLineEdit(p.idNumber());
    idNum->setMaxLength(14);
    idNum->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    
    QLineEdit* spec = new QLineEdit(p.specialization());

    layout->addRow("Full Name:", name);
    layout->addRow("Professor Code:", profCode);
    layout->addRow("National ID:", idNum);
    layout->addRow("Specialization:", spec);
    
    QLabel* noteLabel = new QLabel("Note: Changing the code will update the login username and password.");
    noteLabel->setStyleSheet("color: #666; font-style: italic; font-size: 11px;");
    layout->addRow(noteLabel);

    QPushButton* ok = new QPushButton("Save Changes");
    layout->addRow(ok);
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (profCode->text().isEmpty() || idNum->text().length() != 14) {
            QMessageBox::warning(this, "Validation Error", "Professor Code and 14-digit National ID are required.");
            return;
        }
        
        // Update professor data
        p.setIdNumber(idNum->text());
        p.setSpecialization(spec->text());

        // Update user account if code changed
        bool codeChanged = (profCode->text() != currentCode);
        if (codeChanged) {
            currentUser.setUsername(profCode->text());
            currentUser.setPassword(profCode->text()); // Password matches code
        }
        currentUser.setFullName(name->text());

        if (m_userController.updateUser(currentUser) && m_professorController.updateProfessor(p)) {
            QString msg = "Professor records updated.";
            if (codeChanged) {
                msg += QString("\n\nNew Login Credentials:\nUsername: %1\nPassword: %2")
                       .arg(profCode->text()).arg(profCode->text());
            }
            QMessageBox::information(this, "Success", msg);
            refreshProfessorsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update records.");
        }
    }
}

void AdminPanel::onDeleteProfessor() {
    int row = m_professorsTable->currentRow();
    if (row < 0) return;
    int id = m_professorsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Remove this professor from the faculty?") == QMessageBox::Yes) {
        if (m_professorController.deleteProfessor(id)) {
            QMessageBox::information(this, "Removed", "Professor removed successfully.");
            refreshProfessorsTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove professor.");
        }
    }
}

void AdminPanel::onAddSchedule() {
    QDialog dialog(this);
    dialog.setWindowTitle("New Schedule Slot");
    QFormLayout* layout = new QFormLayout(&dialog);

    QComboBox* course = new QComboBox();
    for(const auto& c : m_courseController.getAllCourses()) course->addItem(c.name(), c.id());
    
    QComboBox* room = new QComboBox();
    for(const auto& r : m_roomController.getAllRooms()) room->addItem(r.name(), r.id());
    
    QComboBox* prof = new QComboBox();
    for(const auto& p : m_professorController.getAllProfessors()) prof->addItem(p.fullName(), p.id());

    QComboBox* day = new QComboBox();
    day->addItems({"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"});

    QTimeEdit* start = new QTimeEdit(QTime(8, 0));
    QTimeEdit* end = new QTimeEdit(QTime(10, 0));

    layout->addRow("Course:", course);
    layout->addRow("Room:", room);
    layout->addRow("Professor:", prof);
    layout->addRow("Day:", day);
    layout->addRow("Start Time:", start);
    layout->addRow("End Time:", end);

    QPushButton* btn = new QPushButton("Create");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        Schedule s;
        s.setCourseId(course->currentData().toInt());
        s.setRoomId(room->currentData().toInt());
        s.setProfessorId(prof->currentData().toInt());
        s.setDayOfWeek(day->currentText());
        s.setStartTime(start->time());
        s.setEndTime(end->time());

        if (m_scheduleController.addSchedule(s)) {
            refreshSchedulesTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add schedule slot. Check if room is already booked.");
        }
    }
}
void AdminPanel::onManageRoomSpecs() {
    int row = m_roomsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Select Room", "Please select a room first.");
        return;
    }
    int roomId = m_roomsTable->item(row, 0)->text().toInt();
    QString roomName = m_roomsTable->item(row, 1)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("Manage Specs for " + roomName);
    dialog.resize(600, 400);
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QTableWidget* table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Prod ID", "Product", "Description"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Load specs
    auto specs = m_roomController.getRoomSpecs(roomId);
    for(const auto& s : specs) {
        int r = table->rowCount();
        table->insertRow(r);
        table->setItem(r, 0, new QTableWidgetItem(s.productId));
        table->setItem(r, 1, new QTableWidgetItem(s.productName));
        table->setItem(r, 2, new QTableWidgetItem(s.description));
    }
    layout->addWidget(table);

    // Inputs
    QHBoxLayout* inputL = new QHBoxLayout();
    QLineEdit* pId = new QLineEdit(); pId->setPlaceholderText("Product ID");
    QLineEdit* pName = new QLineEdit(); pName->setPlaceholderText("Product Name");
    QLineEdit* pDesc = new QLineEdit(); pDesc->setPlaceholderText("Description");
    inputL->addWidget(pId);
    inputL->addWidget(pName);
    inputL->addWidget(pDesc);
    layout->addLayout(inputL);

    QPushButton* addBtn = new QPushButton("Add Item");
    layout->addWidget(addBtn);

    connect(addBtn, &QPushButton::clicked, [&]() {
        if(pId->text().isEmpty() || pName->text().isEmpty()) return;
        m_roomController.addRoomSpec(roomId, pId->text(), pName->text(), pDesc->text());
        Persistence::logChange("RoomSpec", "Add", roomId, pName->text() + " (" + pId->text() + ")");
        
        // Refresh local table
        int r = table->rowCount();
        table->insertRow(r);
        table->setItem(r, 0, new QTableWidgetItem(pId->text()));
        table->setItem(r, 1, new QTableWidgetItem(pName->text()));
        table->setItem(r, 2, new QTableWidgetItem(pDesc->text()));
        
        pId->clear(); pName->clear(); pDesc->clear();
    });
    
    QPushButton* closeBtn = new QPushButton("Close");
    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    dialog.exec();
}
