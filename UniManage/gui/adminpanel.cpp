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

AdminPanel::AdminPanel(int adminId, QWidget *parent)
    : QWidget(parent), m_adminId(adminId)
{
    setupUI();
    setWindowTitle("Admin Panel - University SIS");
    resize(1000, 600);
    
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
    QPushButton* refreshBtn = new QPushButton("Refresh");
    
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::onAddCourse);
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPanel::refreshCoursesTable);
    
    buttonLayout->addWidget(addBtn);
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
    
    QLineEdit* studentNumberEdit = new QLineEdit();
    QLineEdit* idNumberEdit = new QLineEdit();
    QSpinBox* yearSpin = new QSpinBox();
    yearSpin->setRange(1, 4);
    yearSpin->setValue(1);
    
    layout->addRow("Student Number:", studentNumberEdit);
    layout->addRow("ID Number:", idNumberEdit);
    layout->addRow("Year:", yearSpin);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("OK");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addRow(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        StudentData student;
        student.setStudentNumber(studentNumberEdit->text());
        student.setIdNumber(idNumberEdit->text());
        student.setYear(yearSpin->value());
        
        if (m_studentController.addStudent(student)) {
            QMessageBox::information(this, "Success", "Student added successfully");
            refreshStudentsTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add student");
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
    
    QMessageBox::information(this, "Info", "Edit functionality to be implemented");
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
        "Are you sure you want to delete this student?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
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
    QMessageBox::information(this, "Info", "Add course functionality to be implemented");
}
