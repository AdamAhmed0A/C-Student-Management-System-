#include "cli.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <iostream>

Cli::Cli(QObject *parent) : QObject(parent)
{
}

// --- Authentication and Role Handling ---

/**
 * Handles user login by prompting for username and password,
 * authenticates the user, and displays a welcome message on success.
 * @return true if login successful, false otherwise
 */
bool Cli::login() {
    QTextStream qtin(stdin);
    std::cout << "\n-----------------------------------" << std::endl;
    std::cout << "      UniManage CLI Login" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Code (Username): ";
    QString username = qtin.readLine().trimmed();
    std::cout << "National ID (Password): ";
    QString password = qtin.readLine().trimmed();

    m_currentUser = m_controller.authenticate(username, password);

    if (m_currentUser.id() != 0) {
        std::cout << "\n[SUCCESS] Login successful! Welcome, " << m_currentUser.fullName().toStdString() << "." << std::endl;
        std::cout << "Role: " << m_currentUser.role().toUpper().toStdString() << std::endl;
        return true;
    }
    
    std::cout << "\n[ERROR] Login failed: Invalid Code or National ID." << std::endl;
    return false;
}

void Cli::run()
{
    while (true) {
        if (!login()) {
            std::cout << "Would you like to try again? (y/n): ";
            QTextStream qtin(stdin);
            QString retry = qtin.readLine().trimmed().toLower();
            if (retry == "y") {
                continue;
            } else {
                std::cout << "Exiting application. Goodbye!" << std::endl;
                QCoreApplication::exit(0);
                exit(0);
            }
        }

        // Logic branching based on user role
        QString role = m_currentUser.role().toLower();
        if (role == "admin") {
            displayAdminMenu();
        } else if (role == "student") {
            displayStudentDashboard();
        } else if (role == "professor") {
            displayProfessorDashboard();
        } else {
            std::cout << "[ERROR] Unknown user role. Logging out for safety." << std::endl;
        }
    }
}

// --- Dashboard and Menu Displays ---

void Cli::displayAdminMenu()
{
    QTextStream qtin(stdin);
    bool loggedIn = true;

    while(loggedIn) {
        std::cout << "\n===============================" << std::endl;
        std::cout << "      ADMIN CONTROL PANEL" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "1. Student Management" << std::endl;
        std::cout << "2. Course Management" << std::endl;
        std::cout << "3. Professor Management" << std::endl;
        std::cout << "4. User Management" << std::endl;
        std::cout << "5. Logout" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) {
            std::cout << "[!] Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1: displayStudentMenu(); break;
            case 2: displayCourseMenu(); break;
            case 3: displayProfessorMenu(); break;
            case 4: displayUserMenu(); break;
            case 5:
                std::cout << "Logging out administrator..." << std::endl;
                loggedIn = false;
                break;
            case 6:
                std::cout << "Exiting. Goodbye!" << std::endl;
                QCoreApplication::exit(0);
                exit(0);
            default:
                std::cout << "[!] Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void Cli::displayStudentDashboard()
{
    QTextStream qtin(stdin);
    bool loggedIn = true;

    // Fetch student-specific details from data layer
    StudentData student = m_controller.getStudentByUserId(m_currentUser.id());

    while(loggedIn) {
        std::cout << "\n===============================" << std::endl;
        std::cout << "       STUDENT DASHBOARD" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "Welcome, " << m_currentUser.fullName().toStdString() << "!" << std::endl;
        std::cout << "1. View Profile Details" << std::endl;
        std::cout << "2. Logout" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) continue;

        if (choice == 1) {
            if (student.id() == 0) {
                std::cout << "[!] No professional details found for this student account." << std::endl;
            } else {
                std::cout << "\n--- Profile Details ---" << std::endl;
                std::cout << "Name:       " << m_currentUser.fullName().toStdString() << std::endl;
                std::cout << "Student Code: " << student.studentNumber().toStdString() << std::endl;
                std::cout << "National ID:  " << student.idNumber().toStdString() << std::endl;
                std::cout << "Department:   " << student.department().toStdString() << std::endl;
                std::cout << "Join Date:    " << m_currentUser.createdAt().toString("yyyy-MM-dd").toStdString() << std::endl;
                std::cout << "Status:       " << student.status().toStdString() << std::endl;
                std::cout << "-----------------------" << std::endl;
            }
        } else if (choice == 2) {
            std::cout << "Logging out student..." << std::endl;
            loggedIn = false;
        } else if (choice == 3) {
            QCoreApplication::exit(0);
            exit(0);
        }
    }
}

void Cli::displayProfessorDashboard()
{
    QTextStream qtin(stdin);
    bool loggedIn = true;

    // Fetch professor-specific details from data layer
    Professor prof = m_controller.getProfessorByUserId(m_currentUser.id());

    while(loggedIn) {
        std::cout << "\n===============================" << std::endl;
        std::cout << "      PROFESSOR DASHBOARD" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "Welcome, " << m_currentUser.fullName().toStdString() << "!" << std::endl;
        std::cout << "1. View Profile Details" << std::endl;
        std::cout << "2. Logout" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) continue;

        if (choice == 1) {
            if (prof.id() == 0) {
                std::cout << "[!] No professional details found for this professor account." << std::endl;
            } else {
                std::cout << "\n--- Profile Details ---" << std::endl;
                std::cout << "Name:           " << m_currentUser.fullName().toStdString() << std::endl;
                std::cout << "Specialization: " << prof.specialization().toStdString() << std::endl;
                std::cout << "ID Number:      " << prof.idNumber().toStdString() << std::endl;
                std::cout << "Hired Date:     " << m_currentUser.createdAt().toString("yyyy-MM-dd").toStdString() << std::endl;
                std::cout << "-----------------------" << std::endl;
            }
        } else if (choice == 2) {
            std::cout << "Logging out professor..." << std::endl;
            loggedIn = false;
        } else if (choice == 3) {
            QCoreApplication::exit(0);
            exit(0);
        }
    }
}

void Cli::displayStudentMenu()
{
    QTextStream qtin(stdin);
    bool back = false;

    while (!back) {
        std::cout << "\n--- Student Management ---" << std::endl;
        std::cout << "1. List All Students" << std::endl;
        std::cout << "2. Find Student by ID" << std::endl;
        std::cout << "3. Add New Student" << std::endl;
        std::cout << "4. Update Student" << std::endl;
        std::cout << "5. Delete Student" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 0:
                back = true;
                break;
            case 1:
                listAllStudents();
                break;
            case 2:
                findStudentById();
                break;
            case 3:
                addStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void Cli::displayCourseMenu()
{
    QTextStream qtin(stdin);
    bool back = false;

    while (!back) {
        std::cout << "\n--- Course Management ---" << std::endl;
        std::cout << "1. List All Courses" << std::endl;
        std::cout << "2. Find Course by ID" << std::endl;
        std::cout << "3. Add New Course" << std::endl;
        std::cout << "4. Update Course" << std::endl;
        std::cout << "5. Delete Course" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 0:
                back = true;
                break;
            case 1:
                listAllCourses();
                break;
            case 2:
                findCourseById();
                break;
            case 3:
                addCourse();
                break;
            case 4:
                updateCourse();
                break;
            case 5:
                deleteCourse();
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void Cli::displayProfessorMenu()
{
    QTextStream qtin(stdin);
    bool back = false;

    while (!back) {
        std::cout << "\n--- Professor Management ---" << std::endl;
        std::cout << "1. List All Professors" << std::endl;
        std::cout << "2. Find Professor by ID" << std::endl;
        std::cout << "3. Add New Professor" << std::endl;
        std::cout << "4. Update Professor" << std::endl;
        std::cout << "5. Delete Professor" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 0:
                back = true;
                break;
            case 1:
                listAllProfessors();
                break;
            case 2:
                findProfessorById();
                break;
            case 3:
                addProfessor();
                break;
            case 4:
                updateProfessor();
                break;
            case 5:
                deleteProfessor();
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void Cli::displayUserMenu()
{
    QTextStream qtin(stdin);
    bool back = false;

    while (!back) {
        std::cout << "\n--- User Management ---" << std::endl;
        std::cout << "1. List All Users" << std::endl;
        std::cout << "2. Add New User" << std::endl;
        std::cout << "3. Delete User" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";

        QString input = qtin.readLine().trimmed();
        bool ok;
        int choice = input.toInt(&ok);

        if (!ok) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 0:
                back = true;
                break;
            case 1:
                listAllUsers();
                break;
            case 2:
                addUser();
                break;
            case 3:
                deleteUser();
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

// --- Student Functions ---

void Cli::listAllStudents()
{
    QList<StudentData> students = m_controller.getAllStudents();

    if (students.isEmpty()) {
        std::cout << "No students found." << std::endl;
        return;
    }

    std::cout << "\n--- All Students ---" << std::endl;
    printf("%-25s | %-5s | %-15s | %-20s\n", "Name", "ID", "Code", "National ID");
    printf("--------------------------|-------|-----------------|----------------------\n");
    for (const auto &student : students) {
        printf("%-25s | %-5d | %-15s | %-20s\n",
                student.fullName().toStdString().c_str(),
                student.id(),
                student.studentNumber().toStdString().c_str(),
                student.idNumber().toStdString().c_str());
    }
}

void Cli::findStudentById()
{
    QTextStream qtin(stdin);
    std::cout << "Enter student code: ";
    QString studentCode = qtin.readLine().trimmed();

    StudentData student = m_controller.getStudentByStudentNumber(studentCode);

    if (student.id() == 0) {
        std::cout << "Student with code " << studentCode.toStdString() << " not found." << std::endl;
    } else {
        std::cout << "\n--- Student Details ---" << std::endl;
        std::cout << "ID: " << student.id() << std::endl;
        std::cout << "Student Name: " << student.fullName().toStdString().c_str() << std::endl;
        std::cout << "Student Code: " << student.studentNumber().toStdString() << std::endl;
        std::cout << "ID Number: " << student.idNumber().toStdString() << std::endl;
        std::cout << "Department: " << student.department().toStdString() << std::endl;
        std::cout << "Status: " << student.status().toStdString() << std::endl;
    }
}

void Cli::addStudent()
{
    QTextStream qtin(stdin);
    StudentData student;

    std::cout << "--- Add New Student ---" << std::endl;
    
    std::cout << "Full Name: ";
    QString fullName = qtin.readLine().trimmed();

    std::cout << "Student Code (8-digit number): ";
    QString code = qtin.readLine().trimmed();
    student.setStudentNumber(code);

    std::cout << "National ID Number (14 digits): ";
    QString nationalId = qtin.readLine().trimmed();
    student.setIdNumber(nationalId);

    // Create associated User first
    User user;
    user.setFullName(fullName);
    user.setUsername(code);
    user.setPassword(nationalId);
    user.setRole("student");

    if (m_controller.addUser(user)) {
         student.setUserId(user.id());
         std::cout << "User created with ID: " << user.id() << std::endl;
    } else {
         std::cout << "Error: Could not create user for student." << std::endl;
         return;
    }

    std::cout << "Date of Birth (YYYY-MM-DD): ";
    student.setDob(QDateTime::fromString(qtin.readLine().trimmed(), "yyyy-MM-dd"));

    std::cout << "Department: ";
    student.setDepartment(qtin.readLine().trimmed());

    std::cout << "Academic Level ID: ";
    student.setAcademicLevelId(qtin.readLine().trimmed().toInt());

    std::cout << "Section ID: ";
    student.setSectionId(qtin.readLine().trimmed().toInt());

    std::cout << "College ID: ";
    student.setCollegeId(qtin.readLine().trimmed().toInt());

    student.setStatus("active");

    if (m_controller.addStudent(student)) {
        std::cout << "Student added successfully with ID: " << student.id() << "!" << std::endl;
    } else {
        std::cout << "Error: Could not add student." << std::endl;
    }
}

void Cli::updateStudent()
{
    QTextStream qtin(stdin);
    std::cout << "Enter code of student to update: ";
    QString studentCode = qtin.readLine().trimmed();

    StudentData student = m_controller.getStudentByStudentNumber(studentCode);

    if (student.id() == 0) {
        std::cout << "Student with code " << studentCode.toStdString() << " not found." << std::endl;
        return;
    }

    std::cout << "--- Updating Student: " << student.studentNumber().toStdString() << " ---" << std::endl;
    std::cout << "Enter new information or press Enter to keep current value." << std::endl;

    std::cout << "Student Number [" << student.studentNumber().toStdString() << "]: ";
    QString newStudentNum = qtin.readLine().trimmed();
    if (!newStudentNum.isEmpty()) student.setStudentNumber(newStudentNum);

    std::cout << "National ID Number [" << student.idNumber().toStdString() << "]: ";
    QString newIdNum = qtin.readLine().trimmed();
    if (!newIdNum.isEmpty()) student.setIdNumber(newIdNum);

    std::cout << "Date of Birth (YYYY-MM-DD) [" << student.dob().toString("yyyy-MM-dd").toStdString() << "]: ";
    QString newDobStr = qtin.readLine().trimmed();
    if (!newDobStr.isEmpty()) student.setDob(QDateTime::fromString(newDobStr, "yyyy-MM-dd"));

    std::cout << "Department [" << student.department().toStdString() << "]: ";
    QString newDept = qtin.readLine().trimmed();
    if (!newDept.isEmpty()) student.setDepartment(newDept);

    std::cout << "Status [" << student.status().toStdString() << "] (active/inactive): ";
    QString newStatus = qtin.readLine().trimmed();
    if (!newStatus.isEmpty()) student.setStatus(newStatus);

    // Update full name in User
    User user = m_controller.getUserById(student.userId());
    std::cout << "Full Name [" << user.fullName().toStdString() << "]: ";
    QString newFullName = qtin.readLine().trimmed();
    if (!newFullName.isEmpty()) {
        user.setFullName(newFullName);
        m_controller.updateUser(user);
    }

    if (m_controller.updateStudent(student)) {
        std::cout << "Student updated successfully!" << std::endl;
    } else {
        std::cout << "Error: Could not update student." << std::endl;
    }
}

void Cli::deleteStudent()
{
    QTextStream qtin(stdin);
    std::cout << "Enter code of student to delete: ";
    QString studentCode = qtin.readLine().trimmed();

    StudentData student = m_controller.getStudentByStudentNumber(studentCode);
    if (student.id() == 0) {
        std::cout << "Student with code " << studentCode.toStdString() << " not found." << std::endl;
        return;
    }

    std::cout << "Are you sure you want to delete " << student.studentNumber().toStdString() << " (ID: " << student.id() << ")? [y/N]: ";
    QString confirm = qtin.readLine().trimmed().toLower();

    if (confirm == "y") {
        if (m_controller.deleteStudent(student.id())) {
            m_controller.deleteUser(student.userId());
            std::cout << "Student deleted successfully." << std::endl;
        } else {
            std::cout << "Error: Could not delete student." << std::endl;
        }
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
}

// --- Course Functions ---

void Cli::listAllCourses()
{
    QList<Course> courses = m_controller.getAllCourses();

    if (courses.isEmpty()) {
        std::cout << "No courses found." << std::endl;
        return;
    }

    std::cout << "\n--- All Courses ---" << std::endl;
    printf("%-5s | %-20s | %-30s | %-5s\n", "ID", "Name", "Description", "Credit Hours");
    printf("------|----------------------|--------------------------------|-------------\n");
    for (const auto &course : courses) {
        printf("%-5d | %-20s | %-30s | %-5d\n",
                course.id(),
                course.name().toStdString().c_str(),
                course.description().toStdString().c_str(),
                course.creditHours());
    }
}

void Cli::findCourseById()
{
    QTextStream qtin(stdin);
    std::cout << "Enter course ID: ";
    QString courseIdStr = qtin.readLine().trimmed();
    bool ok;
    int courseId = courseIdStr.toInt(&ok);
    if (!ok) {
        std::cout << "Invalid ID. Please enter a numeric ID." << std::endl;
        return;
    }

    Course course = m_controller.getCourseById(courseId);

    if (course.id() == 0) {
        std::cout << "Course with ID " << courseId << " not found." << std::endl;
    } else {
        std::cout << "\n--- Course Details ---" << std::endl;
        std::cout << "ID: " << course.id() << std::endl;
        std::cout << "Name: " << course.name().toStdString() << std::endl;
        std::cout << "Description: " << course.description().toStdString() << std::endl;
        std::cout << "Year Level: " << course.yearLevel() << std::endl;
        std::cout << "Credit Hours: " << course.creditHours() << std::endl;
        std::cout << "Semester ID: " << course.semesterId() << std::endl;
        std::cout << "Course Type: " << course.courseType().toStdString() << std::endl;
    }
}

void Cli::addCourse()
{
    QTextStream qtin(stdin);
    Course course;

    std::cout << "--- Add New Course ---" << std::endl;
    std::cout << "Name: ";
    course.setName(qtin.readLine().trimmed());

    std::cout << "Description: ";
    course.setDescription(qtin.readLine().trimmed());

    std::cout << "Year Level: ";
    course.setYearLevel(qtin.readLine().trimmed().toInt());

    std::cout << "Credit Hours: ";
    course.setCreditHours(qtin.readLine().trimmed().toInt());

    std::cout << "Semester ID: ";
    course.setSemesterId(qtin.readLine().trimmed().toInt());

    std::cout << "Department ID: ";
    course.setDepartmentId(qtin.readLine().trimmed().toInt());

    std::cout << "Max Grade: ";
    course.setMaxGrade(qtin.readLine().trimmed().toInt());

    std::cout << "Course Type (practical/theoretical): ";
    course.setCourseType(qtin.readLine().trimmed());

    if (m_controller.addCourse(course)) {
        std::cout << "Course added successfully with ID: " << course.id() << "!" << std::endl;
    } else {
        std::cout << "Error: Could not add course." << std::endl;
    }
}

void Cli::updateCourse()
{
    QTextStream qtin(stdin);
    std::cout << "Enter ID of course to update: ";
    bool ok;
    int courseId = qtin.readLine().trimmed().toInt(&ok);
    if (!ok) {
        std::cout << "Invalid ID." << std::endl;
        return;
    }

    Course course = m_controller.getCourseById(courseId);

    if (course.id() == 0) {
        std::cout << "Course not found." << std::endl;
        return;
    }

    std::cout << "--- Updating Course: " << course.name().toStdString() << " ---" << std::endl;
    std::cout << "Enter new information or press Enter to keep current value." << std::endl;

    std::cout << "Name [" << course.name().toStdString() << "]: ";
    QString newName = qtin.readLine().trimmed();
    if (!newName.isEmpty()) course.setName(newName);

    std::cout << "Description [" << course.description().toStdString() << "]: ";
    QString newDesc = qtin.readLine().trimmed();
    if (!newDesc.isEmpty()) course.setDescription(newDesc);

    std::cout << "Year Level [" << course.yearLevel() << "]: ";
    QString newYearLevel = qtin.readLine().trimmed();
    if (!newYearLevel.isEmpty()) course.setYearLevel(newYearLevel.toInt());

    std::cout << "Credit Hours [" << course.creditHours() << "]: ";
    QString newCreditHours = qtin.readLine().trimmed();
    if (!newCreditHours.isEmpty()) course.setCreditHours(newCreditHours.toInt());

    std::cout << "Course Type [" << course.courseType().toStdString() << "]: ";
    QString newCourseType = qtin.readLine().trimmed();
    if (!newCourseType.isEmpty()) course.setCourseType(newCourseType);

    if (m_controller.updateCourse(course)) {
        std::cout << "Course updated successfully!" << std::endl;
    } else {
        std::cout << "Error: Could not update course." << std::endl;
    }
}

void Cli::deleteCourse()
{
    QTextStream qtin(stdin);
    std::cout << "Enter ID of course to delete: ";
    bool ok;
    int courseId = qtin.readLine().trimmed().toInt(&ok);
    if (!ok) {
        std::cout << "Invalid ID." << std::endl;
        return;
    }

    Course course = m_controller.getCourseById(courseId);
    if (course.id() == 0) {
        std::cout << "Course not found." << std::endl;
        return;
    }

    std::cout << "Are you sure you want to delete " << course.name().toStdString() << " (ID: " << course.id() << ")? [y/N]: ";
    QString confirm = qtin.readLine().trimmed().toLower();

    if (confirm == "y") {
        if (m_controller.deleteCourse(course.id())) {
            std::cout << "Course deleted successfully." << std::endl;
        } else {
            std::cout << "Error: Could not delete course." << std::endl;
        }
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
}

// --- Professor Functions ---

void Cli::listAllProfessors()
{
    QList<Professor> professors = m_controller.getAllProfessors();

    if (professors.isEmpty()) {
        std::cout << "No professors found." << std::endl;
        return;
    }

    std::cout << "\n--- All Professors ---" << std::endl;
    printf("%-5s | %-25s | %-15s | %-20s | %-20s\n", "ID", "Name", "Code", "National ID", "Specialization");
    printf("------|---------------------------|-----------------|----------------------|----------------------\n");
    for (const auto &prof : professors) {
        printf("%-5d | %-25s | %-15s | %-20s | %-20s\n",
                prof.id(),
                prof.fullName().toStdString().c_str(),
                prof.username().toStdString().c_str(),
                prof.idNumber().toStdString().c_str(),
                prof.specialization().toStdString().c_str());
    }
}

void Cli::findProfessorById()
{
    QTextStream qtin(stdin);
    std::cout << "Enter professor code: ";
    QString profCode = qtin.readLine().trimmed();

    Professor professor = m_controller.getProfessorByUsername(profCode);

    if (professor.id() == 0) {
        std::cout << "Professor with code " << profCode.toStdString() << " not found." << std::endl;
    } else {
        std::cout << "\n--- Professor Details ---" << std::endl;
        std::cout << "ID: " << professor.id() << std::endl;
        std::cout << "Full Name: " << professor.fullName().toStdString() << std::endl;
        std::cout << "Specialization: " << professor.specialization().toStdString() << std::endl;
        std::cout << "Code: " << professor.username().toStdString() << std::endl;
        std::cout << "ID Number: " << professor.idNumber().toStdString() << std::endl;
    }
}

void Cli::addProfessor()
{
    QTextStream qtin(stdin);
    Professor professor;

    std::cout << "--- Add New Professor ---" << std::endl;

    std::cout << "Full Name: ";
    QString fullName = qtin.readLine().trimmed();
    professor.setFullName(fullName);

    std::cout << "Code: ";
    QString code = qtin.readLine().trimmed();

    std::cout << "National ID: ";
    QString nationalId = qtin.readLine().trimmed();
    professor.setIdNumber(nationalId);

    // Create associated User first
    User user;
    user.setFullName(fullName);
    user.setUsername(code);
    user.setPassword(nationalId);
    user.setRole("professor");

    if (m_controller.addUser(user)) {
         professor.setUserId(user.id());
         std::cout << "User created with ID: " << user.id() << std::endl;
    } else {
         std::cout << "Error: Could not create user for professor." << std::endl;
         return;
    }

    std::cout << "Specialization: ";
    professor.setSpecialization(qtin.readLine().trimmed());

    std::cout << "Personal Info: ";
    professor.setPersonalInfo(qtin.readLine().trimmed());

    if (m_controller.addProfessor(professor)) {
        std::cout << "Professor added successfully with ID: " << professor.id() << "!" << std::endl;
    } else {
        std::cout << "Error: Could not add professor." << std::endl;
    }
}

void Cli::updateProfessor()
{
    QTextStream qtin(stdin);
    std::cout << "Enter code of professor to update: ";
    QString profCode = qtin.readLine().trimmed();

    Professor professor = m_controller.getProfessorByUsername(profCode);

    if (professor.id() == 0) {
        std::cout << "Professor with code " << profCode.toStdString() << " not found." << std::endl;
        return;
    }

    std::cout << "--- Updating Professor: " << professor.fullName().toStdString() << " ---" << std::endl;
    std::cout << "Enter new information or press Enter to keep current value." << std::endl;

    std::cout << "Full Name [" << professor.fullName().toStdString() << "]: ";
    QString newName = qtin.readLine().trimmed();
    if (!newName.isEmpty()) professor.setFullName(newName);

    // Update full name in User
    User user = m_controller.getUserById(professor.userId());
    if (!newName.isEmpty()) {
        user.setFullName(newName);
        m_controller.updateUser(user);
    }

    std::cout << "Specialization [" << professor.specialization().toStdString() << "]: ";
    QString newSpec = qtin.readLine().trimmed();
    if (!newSpec.isEmpty()) professor.setSpecialization(newSpec);

    std::cout << "ID Number [" << professor.idNumber().toStdString() << "]: ";
    QString newIdNum = qtin.readLine().trimmed();
    if (!newIdNum.isEmpty()) professor.setIdNumber(newIdNum);

    if (m_controller.updateProfessor(professor)) {
        std::cout << "Professor updated successfully!" << std::endl;
    } else {
        std::cout << "Error: Could not update professor." << std::endl;
    }
}

void Cli::deleteProfessor()
{
    QTextStream qtin(stdin);
    std::cout << "Enter code of professor to delete: ";
    QString profCode = qtin.readLine().trimmed();

    Professor professor = m_controller.getProfessorByUsername(profCode);
    if (professor.id() == 0) {
        std::cout << "Professor with code " << profCode.toStdString() << " not found." << std::endl;
        return;
    }

    std::cout << "Are you sure you want to delete " << professor.fullName().toStdString() << " (ID: " << professor.id() << ")? [y/N]: ";
    QString confirm = qtin.readLine().trimmed().toLower();

    if (confirm == "y") {
        if (m_controller.deleteProfessor(professor.id())) {
            m_controller.deleteUser(professor.userId());
            std::cout << "Professor deleted successfully." << std::endl;
        } else {
            std::cout << "Error: Could not delete professor." << std::endl;
        }
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
}

// --- User Functions ---

void Cli::listAllUsers()
{
    QList<User> users = m_controller.getAllUsers();

    if (users.isEmpty()) {
        std::cout << "No users found." << std::endl;
        return;
    }

    std::cout << "\n--- All Users ---" << std::endl;
    printf("%-5s | %-20s | %-15s | %-10s\n", "ID", "Full Name", "Username", "Role");
    printf("------|----------------------|-----------------|------------\n");
    for (const auto &user : users) {
        printf("%-5d | %-20s | %-15s | %-10s\n",
                user.id(),
                user.fullName().toStdString().c_str(),
                user.username().toStdString().c_str(),
                user.role().toStdString().c_str());
    }
}

void Cli::addUser()
{
    QTextStream qtin(stdin);
    User user;

    std::cout << "--- Add New User ---" << std::endl;
    std::cout << "Full Name: ";
    user.setFullName(qtin.readLine().trimmed());

    std::cout << "Code: ";
    user.setUsername(qtin.readLine().trimmed());

    std::cout << "National ID: ";
    user.setPassword(qtin.readLine().trimmed());

    std::cout << "Role (admin/student/professor): ";
    user.setRole(qtin.readLine().trimmed());

    if (m_controller.addUser(user)) {
        std::cout << "User added successfully with ID: " << user.id() << "!" << std::endl;
    } else {
        std::cout << "Error: Could not add user." << std::endl;
    }
}

void Cli::deleteUser()
{
    QTextStream qtin(stdin);
    std::cout << "Enter ID of user to delete: ";
    bool ok;
    int userId = qtin.readLine().trimmed().toInt(&ok);
    if (!ok) {
        std::cout << "Invalid ID." << std::endl;
        return;
    }

    User user = m_controller.getUserById(userId);
    if (user.id() == 0) {
        std::cout << "User not found." << std::endl;
        return;
    }

    std::cout << "Are you sure you want to delete " << user.fullName().toStdString() << " (ID: " << user.id() << ")? [y/N]: ";
    QString confirm = qtin.readLine().trimmed().toLower();

    if (confirm == "y") {
        if (m_controller.deleteUser(user.id())) {
            std::cout << "User deleted successfully." << std::endl;
        } else {
            std::cout << "Error: Could not delete user." << std::endl;
        }
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
}
