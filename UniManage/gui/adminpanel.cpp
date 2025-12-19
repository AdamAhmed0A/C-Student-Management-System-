#include "adminpanel.h"
#include "stylehelper.h"
#include "loginwindow.h"
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
#include <QRegularExpressionValidator>
#include <QRegularExpression>

AdminPanel::AdminPanel(int adminId, QWidget *parent)
    : QWidget(parent), m_adminId(adminId)
{
    setStyleSheet(StyleHelper::getMainStyle());
    setupUI();
    setWindowTitle("Admin Panel - University Student Information System");
    resize(1280, 850);
    
    // Initial data load
    refreshCollegesTable();
    refreshCoursesTable();
    refreshDepartmentsTable();
    refreshRoomsTable();
    refreshLevelsTable();
    refreshProfessorsTable();
    refreshSchedulesTable();
    refreshStudentsTable();
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
    
    QPushButton* logoutBtn = new QPushButton("Logout System");
    logoutBtn->setObjectName("dangerBtn");
    logoutBtn->setFixedWidth(150);
    connect(logoutBtn, &QPushButton::clicked, this, &AdminPanel::onLogout);
    header->addWidget(logoutBtn);
    
    mainLayout->addLayout(header);
    
    m_tabWidget = new QTabWidget(this);
    
    // Reorganized Tabs as requested
    m_tabWidget->addTab(createFacultiesTab(), "Faculties");
    m_tabWidget->addTab(createCoursesTab(), "Academic Courses");
    m_tabWidget->addTab(createDepartmentsTab(), "Departments");
    m_tabWidget->addTab(createRoomsTab(), "Halls Labs");
    m_tabWidget->addTab(createAcademicSetupTab(), "Academic Setup");
    m_tabWidget->addTab(createProfessorsTab(), "Professor Data");
    m_tabWidget->addTab(createSchedulesTab(), "Academic Schedules");
    m_tabWidget->addTab(createStudentsTab(), "Students");
    
    mainLayout->addWidget(m_tabWidget);
}

QWidget* AdminPanel::createFacultiesTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QPushButton* addBtn = new QPushButton("Add Faculty/College");
    addBtn->setFixedWidth(200);
    m_collegesTable = new QTableWidget();
    m_collegesTable->setColumnCount(3);
    m_collegesTable->setHorizontalHeaderLabels({"ID", "Faculty Name", "Code"});
    m_collegesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_collegesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(addBtn);
    layout->addWidget(m_collegesTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCollege);
    return widget;
}

QWidget* AdminPanel::createDepartmentsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QPushButton* addBtn = new QPushButton("Add Department");
    addBtn->setFixedWidth(200);
    m_departmentsTable = new QTableWidget();
    m_departmentsTable->setColumnCount(4);
    m_departmentsTable->setHorizontalHeaderLabels({"ID", "Dept Name", "Faculty", "Code"});
    m_departmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_departmentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(addBtn);
    layout->addWidget(m_departmentsTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddDepartment);
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
    m_coursesTable->setColumnCount(8);
    m_coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Type", "Max Degree", "Credits", "Level", "Semester", "Description"});
    m_coursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addLayout(btns);
    layout->addWidget(m_coursesTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCourse);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditCourse);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteCourse);
    return widget;
}

QWidget* AdminPanel::createRoomsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add Hall/Lab");
    QPushButton* specBtn = new QPushButton("Manage Specs");
    specBtn->setObjectName("secondaryBtn");
    btns->addWidget(addBtn);
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
    connect(specBtn, &QPushButton::clicked, this, &AdminPanel::onManageRoomSpecs);
    return widget;
}

QWidget* AdminPanel::createAcademicSetupTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QPushButton* addBtn = new QPushButton("Add Academic Level");
    addBtn->setFixedWidth(200);
    m_levelsTable = new QTableWidget();
    m_levelsTable->setColumnCount(3);
    m_levelsTable->setHorizontalHeaderLabels({"ID", "Level Name", "Level Number"});
    m_levelsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_levelsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(addBtn);
    layout->addWidget(m_levelsTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddLevel);
    return widget;
}

QWidget* AdminPanel::createProfessorsTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QPushButton* addBtn = new QPushButton("Register Professor");
    addBtn->setFixedWidth(200);
    m_professorsTable = new QTableWidget();
    m_professorsTable->setColumnCount(4);
    m_professorsTable->setHorizontalHeaderLabels({"ID", "Professor Name", "Specialization", "Title"});
    m_professorsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_professorsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(addBtn);
    layout->addWidget(m_professorsTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddProfessor);
    return widget;
}

QWidget* AdminPanel::createSchedulesTab() {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QPushButton* addBtn = new QPushButton("Assign Schedule Item");
    addBtn->setFixedWidth(200);
    m_schedulesTable = new QTableWidget();
    m_schedulesTable->setColumnCount(7);
    m_schedulesTable->setHorizontalHeaderLabels({"ID", "Course", "Room", "Professor", "Day", "Start", "End"});
    m_schedulesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_schedulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(addBtn);
    layout->addWidget(m_schedulesTable);
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddSchedule);
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
    m_studentsTable->setColumnCount(7);
    m_studentsTable->setHorizontalHeaderLabels({"ID", "Student Code", "Name", "National ID", "Department", "Level", "Status"});
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
    m_studentsTable->setRowCount(0);
    for (const auto& s : m_studentController.getAllStudents()) {
        int r = m_studentsTable->rowCount();
        m_studentsTable->insertRow(r);
        m_studentsTable->setItem(r, 0, new QTableWidgetItem(QString::number(s.id())));
        m_studentsTable->setItem(r, 1, new QTableWidgetItem(s.studentNumber()));
        m_studentsTable->setItem(r, 2, new QTableWidgetItem(s.fullName()));
        m_studentsTable->setItem(r, 3, new QTableWidgetItem(s.idNumber()));
        m_studentsTable->setItem(r, 4, new QTableWidgetItem(s.department()));
        m_studentsTable->setItem(r, 5, new QTableWidgetItem(QString::number(s.academicLevelId())));
        m_studentsTable->setItem(r, 6, new QTableWidgetItem(s.status()));
    }
}

void AdminPanel::refreshCoursesTable() {
    m_coursesTable->setRowCount(0);
    for (const auto& c : m_courseController.getAllCourses()) {
        int r = m_coursesTable->rowCount();
        m_coursesTable->insertRow(r);
        m_coursesTable->setItem(r, 0, new QTableWidgetItem(QString::number(c.id())));
        m_coursesTable->setItem(r, 1, new QTableWidgetItem(c.name()));
        m_coursesTable->setItem(r, 2, new QTableWidgetItem(c.courseType()));
        m_coursesTable->setItem(r, 3, new QTableWidgetItem(QString::number(c.maxGrade())));
        m_coursesTable->setItem(r, 4, new QTableWidgetItem(QString::number(c.creditHours())));
        m_coursesTable->setItem(r, 5, new QTableWidgetItem(QString::number(c.yearLevel())));
        m_coursesTable->setItem(r, 6, new QTableWidgetItem(c.semesterName()));
        m_coursesTable->setItem(r, 7, new QTableWidgetItem(c.description()));
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
        m_professorsTable->setItem(r, 2, new QTableWidgetItem(p.specialization()));
        m_professorsTable->setItem(r, 3, new QTableWidgetItem(p.title()));
    }
}

void AdminPanel::refreshSchedulesTable() {}

void AdminPanel::onAddStudent() {
    QDialog dialog(this);
    dialog.setWindowTitle("Student Registration");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QLineEdit* nationalId = new QLineEdit();
    nationalId->setMaxLength(14);
    nationalId->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    nationalId->setPlaceholderText("Must be 14 digits");
    
    QLineEdit* code = new QLineEdit();
    QComboBox* dept = new QComboBox();
    for(const auto& d : m_departmentController.getAllDepartments()) dept->addItem(d.name(), d.id());
    layout->addRow("Full Name:", name);
    layout->addRow("National ID:", nationalId);
    layout->addRow("Student Code:", code);
    layout->addRow("Department:", dept);
    QPushButton* btn = new QPushButton("Register");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (nationalId->text().length() != 14) {
            QMessageBox::warning(this, "Validation Error", "National ID must be exactly 14 digits.");
            return;
        }
        User u; u.setFullName(name->text()); u.setUsername(code->text()); u.setRole("student"); u.setPassword(code->text());
        if (m_userController.addUser(u)) {
            User cu = m_userController.getUserByUsername(code->text());
            StudentData sd; sd.setUserId(cu.id()); sd.setStudentNumber(code->text()); sd.setIdNumber(nationalId->text()); sd.setDepartmentId(dept->currentData().toInt());
            m_studentController.addStudent(sd);
            refreshStudentsTable();
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
    dialog.setWindowTitle("Edit Student");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit(student.fullName());
    QLineEdit* idEdit = new QLineEdit(student.idNumber());
    idEdit->setMaxLength(14);
    idEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    
    layout->addRow("Name:", nameEdit);
    layout->addRow("ID Number:", idEdit);
    QPushButton* ok = new QPushButton("Save");
    layout->addRow(ok);
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (idEdit->text().length() != 14) {
            QMessageBox::warning(this, "Validation Error", "National ID must be exactly 14 digits.");
            return;
        }
        student.setFullName(nameEdit->text());
        student.setIdNumber(idEdit->text());
        m_studentController.updateStudent(student);
        refreshStudentsTable();
    }
}

void AdminPanel::onDeleteStudent() {
    int row = m_studentsTable->currentRow();
    if (row < 0) return;
    int id = m_studentsTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Delete student?") == QMessageBox::Yes) {
        m_studentController.deleteStudent(id);
        refreshStudentsTable();
    }
}

void AdminPanel::onAddCourse() {
    QDialog dialog(this);
    dialog.setWindowTitle("New Course Entry");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QComboBox* type = new QComboBox(); type->addItems({"Theoretical", "Practical"});
    QComboBox* maxG = new QComboBox(); maxG->addItems({"100", "150"});
    QSpinBox* credits = new QSpinBox(); credits->setRange(1, 10);
    layout->addRow("Course Name:", name);
    layout->addRow("Type:", type);
    layout->addRow("Max Degree:", maxG);
    layout->addRow("Credits:", credits);
    QPushButton* btn = new QPushButton("Create");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        Course c; c.setName(name->text()); c.setCourseType(type->currentText()); c.setMaxGrade(maxG->currentText().toInt()); c.setCreditHours(credits->value());
        m_courseController.addCourse(c);
        refreshCoursesTable();
    }
}

void AdminPanel::onEditCourse() {
    int row = m_coursesTable->currentRow();
    if (row < 0) return;
    int id = m_coursesTable->item(row, 0)->text().toInt();
    Course c = m_courseController.getCourseById(id);
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Course");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit(c.name());
    layout->addRow("Name:", name);
    QPushButton* ok = new QPushButton("Save");
    layout->addRow(ok);
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        c.setName(name->text());
        m_courseController.updateCourse(c);
        refreshCoursesTable();
    }
}

void AdminPanel::onDeleteCourse() {
    int row = m_coursesTable->currentRow();
    if (row < 0) return;
    int id = m_coursesTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm", "Delete course?") == QMessageBox::Yes) {
        m_courseController.deleteCourse(id);
        refreshCoursesTable();
    }
}

void AdminPanel::onAddRoom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Hall/Lab");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QComboBox* type = new QComboBox(); type->addItems({"Hall", "Lab"});
    QSpinBox* cap = new QSpinBox(); cap->setRange(1, 500);
    layout->addRow("Name:", name);
    layout->addRow("Type:", type);
    layout->addRow("Capacity:", cap);
    QPushButton* btn = new QPushButton("Save Room");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        Room r; r.setName(name->text()); r.setType(type->currentText()); r.setCapacity(cap->value());
        m_roomController.addRoom(r);
        refreshRoomsTable();
    }
}

void AdminPanel::onAddCollege() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add College");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QLineEdit* code = new QLineEdit();
    layout->addRow("College Name:", name);
    layout->addRow("Code:", code);
    QPushButton* btn = new QPushButton("Add");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        College c; c.setName(name->text()); c.setCode(code->text());
        m_collegeController.addCollege(c);
        refreshCollegesTable();
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
        Department d; d.setName(name->text()); d.setCollegeId(college->currentData().toInt()); d.setCode(code->text());
        m_departmentController.addDepartment(d);
        refreshDepartmentsTable();
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
        AcademicLevel l; l.setName(name->text()); l.setLevelNumber(num->value());
        m_academicLevelController.addAcademicLevel(l);
        refreshLevelsTable();
    }
}

void AdminPanel::onAddProfessor() {
    QDialog dialog(this);
    dialog.setWindowTitle("Register Professor");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* name = new QLineEdit();
    QLineEdit* idNum = new QLineEdit();
    idNum->setMaxLength(14);
    idNum->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{14}"), &dialog));
    idNum->setPlaceholderText("Exactly 14 digits");
    
    QLineEdit* spec = new QLineEdit();
    QLineEdit* tit = new QLineEdit();
    layout->addRow("Name:", name);
    layout->addRow("National ID:", idNum);
    layout->addRow("Specialization:", spec);
    layout->addRow("Title:", tit);
    QPushButton* btn = new QPushButton("Register");
    layout->addRow(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (idNum->text().length() != 14) {
            QMessageBox::warning(this, "Validation Error", "National ID must be exactly 14 digits.");
            return;
        }
        User u; u.setFullName(name->text()); u.setUsername(name->text().replace(" ", ".")); u.setRole("professor"); u.setPassword("prof123");
        if (m_userController.addUser(u)) {
            User cu = m_userController.getUserByUsername(u.username());
            Professor p; p.setUserId(cu.id()); p.setIdNumber(idNum->text()); p.setSpecialization(spec->text()); p.setTitle(tit->text());
            m_professorController.addProfessor(p);
            refreshProfessorsTable();
        }
    }
}

void AdminPanel::onAddSchedule() {}
void AdminPanel::onManageRoomSpecs() {}
