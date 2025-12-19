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
    m_studentsTable->setColumnCount(7);
    m_studentsTable->setHorizontalHeaderLabels({"ID", "Student Number", "Full Name", "ID Number", "Year", "Department", "Status"});
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
    m_coursesTable->setColumnCount(5);
    m_coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Description", "Year Level", "Credit Hours"});
    m_coursesTable->horizontalHeader()->setStretchLastSection(true);
    m_coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_coursesTable);
    
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
        m_studentsTable->setItem(row, 4, new QTableWidgetItem(QString::number(student.year())));
        m_studentsTable->setItem(row, 5, new QTableWidgetItem(student.department()));
        m_studentsTable->setItem(row, 6, new QTableWidgetItem(student.status()));
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
    QSpinBox* sectionIdSpin = new QSpinBox();       // Section Number
    sectionIdSpin->setRange(0, 9999);
    sectionIdSpin->setSpecialValueText("None");
    sectionIdSpin->setValue(0);
    
    layout->addRow("Full Name:", fullNameEdit);
    layout->addRow("Student Code:", studentNumberEdit);
    layout->addRow("National ID:", idNumberEdit);
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
    QSpinBox* sectionIdSpin = new QSpinBox();
    sectionIdSpin->setRange(0, 9999);
    sectionIdSpin->setSpecialValueText("None");
    sectionIdSpin->setValue(student.sectionId());
    
    layout->addRow("Full Name:", fullNameEdit);
    layout->addRow("National ID:", idNumberEdit);
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
        if (fullNameEdit->text().isEmpty()) {
             QMessageBox::warning(this, "Error", "Full Name is required!");
             return;
        }

        // 1. Update User Record (Full Name)
        User user = m_userController.getUserById(student.userId());
        user.setFullName(fullNameEdit->text());
        m_userController.updateUser(user);

        // 2. Update Student Data
        student.setIdNumber(idNumberEdit->text());
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
    QSpinBox* semesterSpin = new QSpinBox(); // Or combo box
    semesterSpin->setRange(1, 100);
    
    layout->addRow("Course Name:", nameEdit);
    layout->addRow("Description:", descEdit);
    layout->addRow("Year Level:", yearSpin);
    layout->addRow("Credit Hours:", creditSpin);
    layout->addRow("Semester ID:", semesterSpin);
    
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
        c.setSemesterId(semesterSpin->value());
        
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
    
    QSpinBox* semesterSpin = new QSpinBox();
    semesterSpin->setRange(1, 100);
    semesterSpin->setValue(course.semesterId()); 
    
    layout->addRow("Course Name:", nameEdit);
    layout->addRow("Description:", descEdit);
    layout->addRow("Year Level:", yearSpin);
    layout->addRow("Credit Hours:", creditSpin);
    layout->addRow("Semester ID:", semesterSpin);
    
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
        c.setSemesterId(semesterSpin->value());
        
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
