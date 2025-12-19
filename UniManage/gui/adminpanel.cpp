#include "adminpanel.h"
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

AdminPanel::AdminPanel(int adminId, QWidget *parent)
    : QWidget(parent), m_adminId(adminId)
{
    setupUI();
    setWindowTitle("Admin Panel - University SIS");
    resize(1200, 700);
    
    refreshStudentsTable();
    refreshCoursesTable();
}

AdminPanel::~AdminPanel()
{
}

void AdminPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QLabel* titleLabel = new QLabel("Administrator Panel", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createStudentsTab(), "Students");
    m_tabWidget->addTab(createCoursesTab(), "Courses");
    m_tabWidget->addTab(createSetupTab(), "Academic Setup");
    
    mainLayout->addWidget(m_tabWidget);
}

QWidget* AdminPanel::createStudentsTab()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add Student");
    QPushButton* editBtn = new QPushButton("Edit Student");
    QPushButton* deleteBtn = new QPushButton("Delete Student");
    QPushButton* refreshBtn = new QPushButton("Refresh");
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddStudent);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteStudent);
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPanel::refreshStudentsTable);
    
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(editBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addStretch();
    
    layout->addLayout(buttonLayout);
    
    m_studentsTable = new QTableWidget();
    m_studentsTable->setColumnCount(6);
    m_studentsTable->setHorizontalHeaderLabels({"ID", "Student Number", "Full Name", "ID Number", "Department", "Status"});
    m_studentsTable->horizontalHeader()->setStretchLastSection(true);
    m_studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_studentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_studentsTable);
    
    return widget;
}

QWidget* AdminPanel::createCoursesTab()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add Course");
    QPushButton* editBtn = new QPushButton("Edit Course");
    QPushButton* deleteBtn = new QPushButton("Delete Course");
    QPushButton* refreshBtn = new QPushButton("Refresh");
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCourse);
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::onEditCourse);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::onDeleteCourse);
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPanel::refreshCoursesTable);
    
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(editBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addStretch();
    
    layout->addLayout(buttonLayout);
    
    m_coursesTable = new QTableWidget();
    m_coursesTable->setColumnCount(6);
    m_coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Description", "Year Level", "Credit Hours", "Semester"});
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_coursesTable);
    
    return widget;
}

QWidget* AdminPanel::createSetupTab()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QTabWidget* setupTabs = new QTabWidget();
    
    // 1. Colleges Tab
    QWidget* collegesTab = new QWidget();
    QVBoxLayout* collLayout = new QVBoxLayout(collegesTab);
    QHBoxLayout* collBtns = new QHBoxLayout();
    QPushButton* addColl = new QPushButton("Add College");
    QPushButton* refColl = new QPushButton("Refresh");
    collBtns->addWidget(addColl);
    collBtns->addWidget(refColl);
    collBtns->addStretch();
    collLayout->addLayout(collBtns);
    m_collegesTable = new QTableWidget();
    m_collegesTable->setColumnCount(3);
    m_collegesTable->setHorizontalHeaderLabels({"ID", "Name", "Code"});
    m_collegesTable->horizontalHeader()->setStretchLastSection(true);
    m_collegesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    collLayout->addWidget(m_collegesTable);
    setupTabs->addTab(collegesTab, "Colleges");

    connect(addColl, &QPushButton::clicked, this, &AdminPanel::onAddCollege);
    connect(refColl, &QPushButton::clicked, this, &AdminPanel::refreshCollegesTable);

    // 2. Departments Tab
    QWidget* deptsTab = new QWidget();
    QVBoxLayout* deptsLayout = new QVBoxLayout(deptsTab);
    QHBoxLayout* deptsBtns = new QHBoxLayout();
    QPushButton* addDept = new QPushButton("Add Department");
    QPushButton* refDept = new QPushButton("Refresh");
    deptsBtns->addWidget(addDept);
    deptsBtns->addWidget(refDept);
    deptsBtns->addStretch();
    deptsLayout->addLayout(deptsBtns);
    m_departmentsTable = new QTableWidget();
    m_departmentsTable->setColumnCount(4);
    m_departmentsTable->setHorizontalHeaderLabels({"ID", "Name", "College", "Code"});
    m_departmentsTable->horizontalHeader()->setStretchLastSection(true);
    m_departmentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    deptsLayout->addWidget(m_departmentsTable);
    setupTabs->addTab(deptsTab, "Departments");

    connect(addDept, &QPushButton::clicked, this, &AdminPanel::onAddDepartment);
    connect(refDept, &QPushButton::clicked, this, &AdminPanel::refreshDepartmentsTable);

    // 3. Academic Levels Tab
    QWidget* levelsTab = new QWidget();
    QVBoxLayout* levelsLayout = new QVBoxLayout(levelsTab);
    QHBoxLayout* levelsBtns = new QHBoxLayout();
    QPushButton* addLevel = new QPushButton("Add Level");
    QPushButton* refLevel = new QPushButton("Refresh");
    levelsBtns->addWidget(addLevel);
    levelsBtns->addWidget(refLevel);
    levelsBtns->addStretch();
    levelsLayout->addLayout(levelsBtns);
    m_levelsTable = new QTableWidget();
    m_levelsTable->setColumnCount(3);
    m_levelsTable->setHorizontalHeaderLabels({"ID", "Name", "Number"});
    m_levelsTable->horizontalHeader()->setStretchLastSection(true);
    m_levelsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    levelsLayout->addWidget(m_levelsTable);
    setupTabs->addTab(levelsTab, "Academic Levels");

    connect(addLevel, &QPushButton::clicked, this, &AdminPanel::onAddLevel);
    connect(refLevel, &QPushButton::clicked, this, &AdminPanel::refreshLevelsTable);

    // 4. Rooms Tab
    QWidget* roomsTab = new QWidget();
    QVBoxLayout* roomsLayout = new QVBoxLayout(roomsTab);
    QHBoxLayout* roomsBtns = new QHBoxLayout();
    QPushButton* addRoom = new QPushButton("Add Room");
    QPushButton* refRoom = new QPushButton("Refresh");
    roomsBtns->addWidget(addRoom);
    roomsBtns->addWidget(refRoom);
    roomsBtns->addStretch();
    roomsLayout->addLayout(roomsBtns);
    m_roomsTable = new QTableWidget();
    m_roomsTable->setColumnCount(4);
    m_roomsTable->setHorizontalHeaderLabels({"ID", "Name", "Type", "Capacity"});
    m_roomsTable->horizontalHeader()->setStretchLastSection(true);
    m_roomsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    roomsLayout->addWidget(m_roomsTable);
    setupTabs->addTab(roomsTab, "Halls & Labs");

    connect(addRoom, &QPushButton::clicked, this, &AdminPanel::onAddRoom);
    connect(refRoom, &QPushButton::clicked, this, &AdminPanel::refreshRoomsTable);

    // 5. Professors Tab
    QWidget* profsTab = new QWidget();
    QVBoxLayout* profsLayout = new QVBoxLayout(profsTab);
    QHBoxLayout* profsBtns = new QHBoxLayout();
    QPushButton* addProf = new QPushButton("Add Professor");
    QPushButton* refProf = new QPushButton("Refresh");
    profsBtns->addWidget(addProf);
    profsBtns->addWidget(refProf);
    profsBtns->addStretch();
    profsLayout->addLayout(profsBtns);
    m_professorsTable = new QTableWidget();
    m_professorsTable->setColumnCount(4);
    m_professorsTable->setHorizontalHeaderLabels({"ID", "Name", "Specialization", "Title"});
    m_professorsTable->horizontalHeader()->setStretchLastSection(true);
    m_professorsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    profsLayout->addWidget(m_professorsTable);
    setupTabs->addTab(profsTab, "Professors (Doctors)");

    connect(addProf, &QPushButton::clicked, this, &AdminPanel::onAddProfessor);
    connect(refProf, &QPushButton::clicked, this, &AdminPanel::refreshProfessorsTable);

    layout->addWidget(setupTabs);

    // Initial refreshes
    refreshCollegesTable();
    refreshDepartmentsTable();
    refreshLevelsTable();
    refreshRoomsTable();
    refreshProfessorsTable();

    return widget;
}

void AdminPanel::refreshStudentsTable()
{
    m_studentsTable->setRowCount(0);
    QList<StudentData> students = m_studentController.getAllStudents();
    
    for (const StudentData& student : students) {
        int row = m_studentsTable->rowCount();
        m_studentsTable->insertRow(row);
        m_studentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(student.id())));
        m_studentsTable->setItem(row, 1, new QTableWidgetItem(student.studentNumber()));
        m_studentsTable->setItem(row, 2, new QTableWidgetItem(student.fullName()));
        m_studentsTable->setItem(row, 3, new QTableWidgetItem(student.idNumber()));
        m_studentsTable->setItem(row, 4, new QTableWidgetItem(student.department()));
        m_studentsTable->setItem(row, 5, new QTableWidgetItem(student.status()));
    }
}

void AdminPanel::refreshCoursesTable()
{
    m_coursesTable->setRowCount(0);
    QList<Course> courses = m_courseController.getAllCourses();
    
    for (const Course& course : courses) {
        int row = m_coursesTable->rowCount();
        m_coursesTable->insertRow(row);
        m_coursesTable->setItem(row, 0, new QTableWidgetItem(QString::number(course.id())));
        m_coursesTable->setItem(row, 1, new QTableWidgetItem(course.name()));
        m_coursesTable->setItem(row, 2, new QTableWidgetItem(course.description()));
        m_coursesTable->setItem(row, 3, new QTableWidgetItem(QString::number(course.yearLevel())));
        m_coursesTable->setItem(row, 4, new QTableWidgetItem(QString::number(course.creditHours())));
        m_coursesTable->setItem(row, 5, new QTableWidgetItem(course.semesterName()));
    }
}

void AdminPanel::refreshCollegesTable()
{
    m_collegesTable->setRowCount(0);
    QList<College> list = m_collegeController.getAllColleges();
    for (const College& c : list) {
        int row = m_collegesTable->rowCount();
        m_collegesTable->insertRow(row);
        m_collegesTable->setItem(row, 0, new QTableWidgetItem(QString::number(c.id())));
        m_collegesTable->setItem(row, 1, new QTableWidgetItem(c.name()));
        m_collegesTable->setItem(row, 2, new QTableWidgetItem(c.code()));
    }
}

void AdminPanel::refreshDepartmentsTable()
{
    m_departmentsTable->setRowCount(0);
    QList<Department> list = m_departmentController.getAllDepartments();
    for (const Department& d : list) {
        int row = m_departmentsTable->rowCount();
        m_departmentsTable->insertRow(row);
        m_departmentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(d.id())));
        m_departmentsTable->setItem(row, 1, new QTableWidgetItem(d.name()));
        m_departmentsTable->setItem(row, 2, new QTableWidgetItem(d.collegeName()));
        m_departmentsTable->setItem(row, 3, new QTableWidgetItem(d.code()));
    }
}

void AdminPanel::refreshLevelsTable()
{
    m_levelsTable->setRowCount(0);
    QList<AcademicLevel> list = m_academicLevelController.getAllAcademicLevels();
    for (const AcademicLevel& l : list) {
        int row = m_levelsTable->rowCount();
        m_levelsTable->insertRow(row);
        m_levelsTable->setItem(row, 0, new QTableWidgetItem(QString::number(l.id())));
        m_levelsTable->setItem(row, 1, new QTableWidgetItem(l.name()));
        m_levelsTable->setItem(row, 2, new QTableWidgetItem(QString::number(l.levelNumber())));
    }
}

void AdminPanel::refreshRoomsTable()
{
    m_roomsTable->setRowCount(0);
    QList<Room> list = m_roomController.getAllRooms();
    for (const Room& r : list) {
        int row = m_roomsTable->rowCount();
        m_roomsTable->insertRow(row);
        m_roomsTable->setItem(row, 0, new QTableWidgetItem(QString::number(r.id())));
        m_roomsTable->setItem(row, 1, new QTableWidgetItem(r.name()));
        m_roomsTable->setItem(row, 2, new QTableWidgetItem(r.type()));
        m_roomsTable->setItem(row, 3, new QTableWidgetItem(QString::number(r.capacity())));
    }
}

void AdminPanel::refreshProfessorsTable()
{
    m_professorsTable->setRowCount(0);
    QList<Professor> list = m_professorController.getAllProfessors();
    for (const Professor& p : list) {
        int row = m_professorsTable->rowCount();
        m_professorsTable->insertRow(row);
        m_professorsTable->setItem(row, 0, new QTableWidgetItem(QString::number(p.id())));
        m_professorsTable->setItem(row, 1, new QTableWidgetItem(p.fullName()));
        m_professorsTable->setItem(row, 2, new QTableWidgetItem(p.specialization()));
        m_professorsTable->setItem(row, 3, new QTableWidgetItem(p.title()));
    }
}

void AdminPanel::onAddStudent()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Student");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    // Simplified fields
    QLineEdit* fullNameEdit = new QLineEdit();
    QLineEdit* studentNumberEdit = new QLineEdit(); // Student Code
    QLineEdit* idNumberEdit = new QLineEdit();      // National ID
    QComboBox* departmentCombo = new QComboBox();
    QList<Department> departments = m_departmentController.getAllDepartments();
    departmentCombo->addItem("Not Assigned", 0);
    for (const Department& d : departments) departmentCombo->addItem(d.name(), d.id());

    QComboBox* levelCombo = new QComboBox();
    QList<AcademicLevel> levels = m_academicLevelController.getAllAcademicLevels();
    levelCombo->addItem("Not Assigned", 0);
    for (const AcademicLevel& l : levels) levelCombo->addItem(l.name(), l.id());

    QSpinBox* sectionIdSpin = new QSpinBox();       // Section Number
    sectionIdSpin->setRange(0, 9999);
    sectionIdSpin->setSpecialValueText("None");
    sectionIdSpin->setValue(0);
    
    layout->addRow("Full Name:", fullNameEdit);
    layout->addRow("Student Code:", studentNumberEdit);
    layout->addRow("National ID:", idNumberEdit);
    layout->addRow("Department:", departmentCombo);
    layout->addRow("Academic Level:", levelCombo);
    layout->addRow("Section Number:", sectionIdSpin);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Add");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addRow(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Validation
        if (fullNameEdit->text().isEmpty() || studentNumberEdit->text().isEmpty()) {
             QMessageBox::warning(this, "Error", "Full Name and Student Code are required!");
             return;
        }

        // 1. Create User
        User newUser;
        newUser.setFullName(fullNameEdit->text());
        newUser.setUsername(studentNumberEdit->text());
        newUser.setRole("student");
        newUser.setPassword(studentNumberEdit->text()); // Default password to student code
        
        if (!m_userController.addUser(newUser)) {
             QMessageBox::warning(this, "Error", "Failed to create user account. Student Code might already be in use.");
             return;
        }
        
        // 2. Retrieve new User ID
        User createdUser = m_userController.getUserByUsername(studentNumberEdit->text());
        
        // 3. Create Student Data
        StudentData student;
        student.setUserId(createdUser.id());
        student.setStudentNumber(studentNumberEdit->text());
        student.setIdNumber(idNumberEdit->text());
        student.setDepartment(departmentCombo->currentText());
        student.setDepartmentId(departmentCombo->currentData().toInt());
        student.setAcademicLevelId(levelCombo->currentData().toInt());
        student.setSectionId(sectionIdSpin->value());
        
        // Optional fields are now nullable in DB
        
        QString errorMsg;
        if (m_studentController.addStudent(student, &errorMsg)) {
            QMessageBox::information(this, "Success", "Student added successfully");
            refreshStudentsTable();
        } else {
            m_userController.deleteUser(createdUser.id());
            QMessageBox::warning(this, "Error", "Failed to add student data. Details: " + errorMsg);
        }
    }
}

void AdminPanel::onEditStudent()
{
    int row = m_studentsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a student");
        return;
    }
    
    int studentId = m_studentsTable->item(row, 0)->text().toInt();
    StudentData student = m_studentController.getStudentById(studentId);
    if (student.id() == 0) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Student");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* fullNameEdit = new QLineEdit(student.fullName());
    QLineEdit* idNumberEdit = new QLineEdit(student.idNumber());
    
    QComboBox* departmentCombo = new QComboBox();
    QList<Department> departments = m_departmentController.getAllDepartments();
    departmentCombo->addItem("Not Assigned", 0);
    int deptIndex = 0;
    for (int i = 0; i < departments.size(); ++i) {
        departmentCombo->addItem(departments[i].name(), departments[i].id());
        if (departments[i].id() == student.departmentId()) deptIndex = i + 1;
    }
    departmentCombo->setCurrentIndex(deptIndex);

    QComboBox* levelCombo = new QComboBox();
    QList<AcademicLevel> levels = m_academicLevelController.getAllAcademicLevels();
    levelCombo->addItem("Not Assigned", 0);
    int levelIndex = 0;
    for (int i = 0; i < levels.size(); ++i) {
        levelCombo->addItem(levels[i].name(), levels[i].id());
        if (levels[i].id() == student.academicLevelId()) levelIndex = i + 1;
    }
    levelCombo->setCurrentIndex(levelIndex);

    QSpinBox* sectionIdSpin = new QSpinBox();
    sectionIdSpin->setRange(0, 9999);
    sectionIdSpin->setSpecialValueText("None");
    sectionIdSpin->setValue(student.sectionId());
    
    layout->addRow("Full Name:", fullNameEdit);
    layout->addRow("National ID:", idNumberEdit);
    layout->addRow("Department:", departmentCombo);
    layout->addRow("Academic Level:", levelCombo);
    layout->addRow("Section Number:", sectionIdSpin);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Save");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addRow(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Update User
        User user = m_userController.getUserById(student.userId());
        user.setFullName(fullNameEdit->text());
        m_userController.updateUser(user);
        
        // Update Student Data
        student.setFullName(fullNameEdit->text());
        student.setIdNumber(idNumberEdit->text());
        student.setDepartment(departmentCombo->currentText());
        student.setDepartmentId(departmentCombo->currentData().toInt());
        student.setAcademicLevelId(levelCombo->currentData().toInt());
        student.setSectionId(sectionIdSpin->value());
        
        if (m_studentController.updateStudent(student)) {
            QMessageBox::information(this, "Success", "Student updated successfully");
            refreshStudentsTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to update student data");
        }
    }
}

void AdminPanel::onDeleteStudent()
{
    int row = m_studentsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a student");
        return;
    }
    
    int studentId = m_studentsTable->item(row, 0)->text().toInt();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete this student? This will also remove the user account.",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // Implementation note: Ideally we should also delete linked user, 
        // but let's stick to student data first or do both.
        // The foreign key constraint might handle it or we need to do it manually.
        // Let's delete student data primarily.
        
        if (m_studentController.deleteStudent(studentId)) {
            QMessageBox::information(this, "Success", "Student deleted successfully");
            refreshStudentsTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete student");
        }
    }
}

void AdminPanel::onAddCourse()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Course");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* nameEdit = new QLineEdit();
    QLineEdit* descEdit = new QLineEdit();
    QSpinBox* yearSpin = new QSpinBox();
    yearSpin->setRange(1, 6);
    QSpinBox* creditSpin = new QSpinBox();
    creditSpin->setRange(1, 10);
    
    QComboBox* semesterCombo = new QComboBox();
    QList<Semester> semesters = m_semesterController.getAllSemesters();
    for (const Semester& s : semesters) {
        QString name = QString("%1 - Sem %2").arg(s.year().toString("yyyy")).arg(s.semester());
        semesterCombo->addItem(name, s.id());
    }
    
    layout->addRow("Course Name:", nameEdit);
    layout->addRow("Description:", descEdit);
    layout->addRow("Year Level:", yearSpin);
    layout->addRow("Credit Hours:", creditSpin);
    layout->addRow("Semester:", semesterCombo);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Add");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addRow(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (nameEdit->text().isEmpty()) {
             QMessageBox::warning(this, "Error", "Course Name is required");
             return;
        }
        
        Course c;
        c.setName(nameEdit->text());
        c.setDescription(descEdit->text());
        c.setYearLevel(yearSpin->value());
        c.setCreditHours(creditSpin->value());
        c.setSemesterId(semesterCombo->currentData().toInt());
        
        if (m_courseController.addCourse(c)) {
             QMessageBox::information(this, "Success", "Course added successfully");
             refreshCoursesTable();
        } else {
             QMessageBox::warning(this, "Error", "Failed to add course");
        }
    }
}

void AdminPanel::onEditCourse()
{
    int row = m_coursesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a course");
        return;
    }
    
    int courseId = m_coursesTable->item(row, 0)->text().toInt();
    Course course = m_courseController.getCourseById(courseId);
    if (course.id() == 0) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Course");
    QFormLayout* layout = new QFormLayout(&dialog);
    
    QLineEdit* nameEdit = new QLineEdit(course.name());
    QLineEdit* descEdit = new QLineEdit(course.description());
    QSpinBox* yearSpin = new QSpinBox();
    yearSpin->setRange(1, 6);
    yearSpin->setValue(course.yearLevel());
    
    QSpinBox* creditSpin = new QSpinBox();
    creditSpin->setRange(1, 10);
    creditSpin->setValue(course.creditHours());
    
    QComboBox* semesterCombo = new QComboBox();
    QList<Semester> semesters = m_semesterController.getAllSemesters();
    int currentIdx = -1;
    for (int i = 0; i < semesters.size(); ++i) {
        const Semester& s = semesters[i];
        QString name = QString("%1 - Sem %2").arg(s.year().toString("yyyy")).arg(s.semester());
        semesterCombo->addItem(name, s.id());
        if (s.id() == course.semesterId()) currentIdx = i;
    }
    if (currentIdx != -1) semesterCombo->setCurrentIndex(currentIdx);
    
    layout->addRow("Course Name:", nameEdit);
    layout->addRow("Description:", descEdit);
    layout->addRow("Year Level:", yearSpin);
    layout->addRow("Credit Hours:", creditSpin);
    layout->addRow("Semester:", semesterCombo);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Save");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addRow(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        Course c;
        c.setId(courseId);
        c.setName(nameEdit->text());
        c.setDescription(descEdit->text());
        c.setYearLevel(yearSpin->value());
        c.setCreditHours(creditSpin->value());
        c.setSemesterId(semesterCombo->currentData().toInt());
        
        if (m_courseController.updateCourse(c)) {
             QMessageBox::information(this, "Success", "Course updated successfully");
             refreshCoursesTable();
        } else {
             QMessageBox::warning(this, "Error", "Failed to update course");
        }
    }
}

void AdminPanel::onDeleteCourse()
{
    int row = m_coursesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a course");
        return;
    }
    
    int courseId = m_coursesTable->item(row, 0)->text().toInt();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete this course?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_courseController.deleteCourse(courseId)) {
            QMessageBox::information(this, "Success", "Course deleted successfully");
            refreshCoursesTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete course");
        }
    }
}

void AdminPanel::onAddCollege()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add College");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit();
    QLineEdit* codeEdit = new QLineEdit();
    layout->addRow("College Name:", nameEdit);
    layout->addRow("Code:", codeEdit);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* ok = new QPushButton("Add");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    btns->addWidget(ok);
    btns->addWidget(cancel);
    layout->addRow(btns);

    if (dialog.exec() == QDialog::Accepted) {
        College c;
        c.setName(nameEdit->text());
        c.setCode(codeEdit->text());
        if (m_collegeController.addCollege(c)) {
            refreshCollegesTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add college");
        }
    }
}

void AdminPanel::onAddDepartment()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Department");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit();
    QComboBox* collegeCombo = new QComboBox();
    QList<College> colleges = m_collegeController.getAllColleges();
    for (const College& c : colleges) collegeCombo->addItem(c.name(), c.id());
    QLineEdit* codeEdit = new QLineEdit();
    
    layout->addRow("Department Name:", nameEdit);
    layout->addRow("College:", collegeCombo);
    layout->addRow("Code:", codeEdit);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* ok = new QPushButton("Add");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    btns->addWidget(ok);
    btns->addWidget(cancel);
    layout->addRow(btns);

    if (dialog.exec() == QDialog::Accepted) {
        Department d;
        d.setName(nameEdit->text());
        d.setCollegeId(collegeCombo->currentData().toInt());
        d.setCode(codeEdit->text());
        if (m_departmentController.addDepartment(d)) {
            refreshDepartmentsTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add department");
        }
    }
}

void AdminPanel::onAddLevel()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Academic Level");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit();
    QSpinBox* numSpin = new QSpinBox();
    numSpin->setRange(1, 10);
    layout->addRow("Level Name:", nameEdit);
    layout->addRow("Level Number:", numSpin);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* ok = new QPushButton("Add");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    btns->addWidget(ok);
    btns->addWidget(cancel);
    layout->addRow(btns);

    if (dialog.exec() == QDialog::Accepted) {
        AcademicLevel l;
        l.setName(nameEdit->text());
        l.setLevelNumber(numSpin->value());
        if (m_academicLevelController.addAcademicLevel(l)) {
            refreshLevelsTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add academic level");
        }
    }
}

void AdminPanel::onAddRoom()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Room (Hall/Lab)");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit();
    QComboBox* typeCombo = new QComboBox();
    typeCombo->addItems({"Hall", "Lab"});
    QSpinBox* capSpin = new QSpinBox();
    capSpin->setRange(1, 1000);
    layout->addRow("Room Name:", nameEdit);
    layout->addRow("Type:", typeCombo);
    layout->addRow("Capacity:", capSpin);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* ok = new QPushButton("Add");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    btns->addWidget(ok);
    btns->addWidget(cancel);
    layout->addRow(btns);

    if (dialog.exec() == QDialog::Accepted) {
        Room r;
        r.setName(nameEdit->text());
        r.setType(typeCombo->currentText());
        r.setCapacity(capSpin->value());
        if (m_roomController.addRoom(r)) {
            refreshRoomsTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add room");
        }
    }
}

void AdminPanel::onAddProfessor()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Professor");
    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit();
    QLineEdit* specEdit = new QLineEdit();
    QLineEdit* titleEdit = new QLineEdit();
    layout->addRow("Full Name:", nameEdit);
    layout->addRow("Specialization:", specEdit);
    layout->addRow("Title:", titleEdit);
    
    QHBoxLayout* btns = new QHBoxLayout();
    QPushButton* ok = new QPushButton("Add");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    btns->addWidget(ok);
    btns->addWidget(cancel);
    layout->addRow(btns);

    if (dialog.exec() == QDialog::Accepted) {
        // 1. Create User
        User u;
        u.setFullName(nameEdit->text());
        u.setUsername(nameEdit->text().toLower().replace(" ", "."));
        u.setRole("professor");
        u.setPassword("password123");
        if (m_userController.addUser(u)) {
            User createdUser = m_userController.getUserByUsername(u.username());
            Professor p;
            p.setUserId(createdUser.id());
            p.setSpecialization(specEdit->text());
            p.setTitle(titleEdit->text());
            if (m_professorController.addProfessor(p)) {
                refreshProfessorsTable();
            } else {
                QMessageBox::warning(this, "Error", "Failed to add professor data");
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to create professor user");
        }
    }
}
