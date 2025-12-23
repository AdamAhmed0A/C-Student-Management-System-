#include <QCoreApplication>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <string>
#include <iomanip>

#include "../../database/dbconnection.h"
#include "../../database/queries.h"
#include "../../controllers/usercontroller.h"
#include "../../controllers/professorcontroller.h"
#include "../../controllers/studentcontroller.h"
#include "../../controllers/enrollmentcontroller.h"
#include "../../controllers/coursecontroller.h"
#include "../../controllers/schedulecontroller.h"
#include "../../controllers/academic_level_controller.h"
#include "../../controllers/roomcontroller.h"
#include "../../controllers/departmentcontroller.h"
#include "../../controllers/collegecontroller.h"
#include "../../controllers/semestercontroller.h"
#include "../../controllers/calendarcontroller.h"

#include "../../models/user.h"
#include "../../models/professor.h"
#include "../../models/student_data.h"
#include "../../models/enrollment.h"
#include "../../models/course.h"
#include "../../models/room.h"
#include "../../models/academic_level.h"

using namespace std;

// Text output helper
QTextStream out(stdout);
QTextStream in(stdin);

class CLIProject {
public:
    CLIProject() {
        if (!DBConnection::instance().init()) {
            out << "Failed to connect to database!" << endl;
            exit(1);
        }
    }

    void run() {
        while (true) {
            showLoginMenu();
        }
    }

private:
    UserController m_userController;
    ProfessorController m_professorController;
    EnrollmentController m_enrollmentController;
    CourseController m_courseController;
    ScheduleController m_scheduleController;
    AcademicLevelController m_academicLevelController;
    RoomController m_roomController;
    DepartmentController m_departmentController;
    CollegeController m_collegeController;
    SemesterController m_semesterController;
    CalendarController m_calendarController;
    StudentController m_studentController;

    User m_currentUser;

    void showLoginMenu() {
        out << "\n==============================================" << endl;
        out << "   UNIVERSITY MANAGEMENT SYSTEM - CLI VERSION" << endl;
        out << "==============================================" << endl;
        out << "1. Login" << endl;
        out << "2. Exit" << endl;
        out << "Select Option: ";
        out.flush();

        QString choice = in.readLine().trimmed();
        if (choice == "1") {
            login();
        } else if (choice == "2") {
            out << "Goodbye!" << endl;
            exit(0);
        } else {
            out << "Invalid choice." << endl;
        }
    }

    void login() {
        out << "Username: "; out.flush();
        QString username = in.readLine().trimmed();
        out << "Password: "; out.flush();
        QString password = in.readLine().trimmed();

        QString error;
        m_currentUser = m_userController.login(username, password, &error);

        if (m_currentUser.id() > 0) {
            out << "\nLogin Successful! Welcome, " << m_currentUser.fullName() << endl;
            if (m_currentUser.role().toLower() == "admin") {
                adminDashboard();
            } else if (m_currentUser.role().toLower() == "professor") {
                professorDashboard();
            } else if (m_currentUser.role().toLower() == "student") {
                studentDashboard();
            } else {
                out << "Unknown role: " << m_currentUser.role() << endl;
            }
        } else {
            out << "Login Failed: " << error << endl;
        }
    }

    // --- ADMIN DASHBOARD ---
    void adminDashboard() {
        while (true) {
            out << "\n--- ADMIN DASHBOARD ---" << endl;
            out << "1. Manage Students" << endl;
            out << "2. Manage Professors" << endl;
            out << "3. Manage Courses" << endl;
            out << "4. Manage Facilities (Halls/Labs)" << endl;
            out << "5. Manage Academic Setup (Levels/Colleges/Depts)" << endl;
            out << "6. Logout" << endl;
            out << "Select Option: "; out.flush();

            QString choice = in.readLine().trimmed();
            if (choice == "1") manageStudents();
            else if (choice == "2") manageProfessors();
            else if (choice == "3") manageCourses();
            else if (choice == "4") manageFacilities();
            else if (choice == "5") manageAcademicSetup();
            else if (choice == "6") return;
            else out << "Invalid option." << endl;
        }
    }

    void manageStudents() {
        out << "\n--- STUDENT MANAGEMENT ---" << endl;
        QList<StudentData> students = m_studentController.getAllStudents();
        out << left << qSetFieldWidth(10) << "ID" << qSetFieldWidth(30) << "Name" << qSetFieldWidth(20) << "Code" << qSetFieldWidth(0) << endl;
        for (const auto& s : students) {
             out << left << qSetFieldWidth(10) << s.id() << qSetFieldWidth(30) << s.fullName() << qSetFieldWidth(20) << s.studentNumber() << qSetFieldWidth(0) << endl;
        }
        out << "\n[B] Back: "; out.flush();
        in.readLine();
    }

    void manageFacilities() {
        out << "\n--- FACILITIES MANAGEMENT ---" << endl;
        out << "1. View Lecture Halls" << endl;
        out << "2. View Computer Labs" << endl;
        out << "3. Back" << endl;
        out << "Select: "; out.flush();
        QString sc = in.readLine();
        if (sc == "1") {
            auto rooms = m_roomController.getAllRooms();
            out << "\nLECTURE HALLS:" << endl;
            for (const auto& r : rooms) {
                if (r.type().compare("Hall", Qt::CaseInsensitive) == 0) {
                    out << " - " << r.name() << " (Code: " << r.code() << ", Capacity: " << r.capacity() << ")" << endl;
                }
            }
        } else if (sc == "2") {
            auto rooms = m_roomController.getAllRooms();
            out << "\nCOMPUTER LABS:" << endl;
            for (const auto& r : rooms) {
                if (r.type().compare("Lab", Qt::CaseInsensitive) == 0) {
                    out << " - " << r.name() << " (Code: " << r.code() << ", PCs: " << r.computersCount() << ")" << endl;
                }
            }
        }
        out << "\n[B] Back: "; out.flush();
        in.readLine();
    }

    void manageAcademicSetup() {
        out << "\n--- ACADEMIC SETUP ---" << endl;
        out << "Academic Levels:" << endl;
        for (const auto& l : m_academicLevelController.getAllAcademicLevels()) {
            out << " - " << l.name() << " (Year " << l.levelNumber() << ")" << endl;
        }
        out << "\n[B] Back: "; out.flush();
        in.readLine();
    }

    void manageProfessors() { 
        out << "\n--- PROFESSOR MANAGEMENT ---" << endl;
        auto profs = m_professorController.getAllProfessors();
        for(const auto& p : profs) {
            out << " - " << p.fullName() << " (" << p.specialization() << ")" << endl;
        }
        out << "\n[B] Back: "; out.flush();
        in.readLine();
    }
    
    void manageCourses() { 
        out << "\n--- COURSE MANAGEMENT ---" << endl;
        auto courses = m_courseController.getAllCourses();
        for(const auto& c : courses) {
            out << " - " << c.name() << " (" << c.courseType() << ")" << endl;
        }
        out << "\n[B] Back: "; out.flush();
        in.readLine();
    }

    // --- PROFESSOR DASHBOARD ---
    void professorDashboard() {
        Professor prof = m_professorController.getProfessorByUserId(m_currentUser.id());
        while (true) {
            out << "\n--- PROFESSOR DASHBOARD: " << prof.fullName() << " ---" << endl;
            out << "1. View My Courses" << endl;
            out << "2. View Student Grades for a Course" << endl;
            out << "3. Logout" << endl;
            out << "Select Option: "; out.flush();

            QString choice = in.readLine().trimmed();
            if (choice == "1") {
                auto courses = m_courseController.getCoursesByProfessor(prof.id());
                for (const auto& c : courses) out << " - " << c.name() << " (" << c.courseType() << ")" << endl;
                out << "\n[B] Back: "; out.flush(); in.readLine();
            } else if (choice == "2") {
                auto courses = m_courseController.getCoursesByProfessor(prof.id());
                out << "Select Course Index (0 to " << courses.size()-1 << "): "; out.flush();
                int idx = in.readLine().toInt();
                if(idx >=0 && idx < courses.size()) {
                    auto enrolls = m_enrollmentController.getEnrollmentsByCourse(courses[idx].id());
                    for(const auto& e : enrolls) {
                        out << " - " << e.studentName() << ": " << e.totalGrade() << endl;
                    }
                }
                out << "\n[B] Back: "; out.flush(); in.readLine();
            } else if (choice == "3") return;
        }
    }

    // --- STUDENT DASHBOARD ---
    void studentDashboard() {
        StudentData stud = m_studentController.getStudentByUserId(m_currentUser.id());
        while (true) {
            out << "\n--- STUDENT DASHBOARD: " << m_currentUser.fullName() << " ---" << endl;
            out << "1. View My Grades (Non-zero only)" << endl;
            out << "2. View My Schedule" << endl;
            out << "3. Logout" << endl;
            out << "Select Option: "; out.flush();

            QString choice = in.readLine().trimmed();
            if (choice == "1") {
                auto enrolls = m_enrollmentController.getEnrollmentsByStudent(stud.id());
                out << "\n--- MY GRADES ---" << endl;
                for (const auto& e : enrolls) {
                    if (e.assignment1Grade() == 0 && e.assignment2Grade() == 0 && 
                        e.courseworkGrade() == 0 && e.finalExamGrade() == 0 && 
                        e.experienceGrade() == 0) continue;
                    
                    out << "Course: " << e.courseName() << endl;
                    out << "   Grade: " << e.totalGrade() << " (" << e.letterGrade() << ")" << endl;
                }
                out << "\n[B] Back: "; out.flush(); in.readLine();
            } else if (choice == "2") {
                // Simplified schedule view
                out << "Schedule view simplified for CLI..." << endl;
                out << "\n[B] Back: "; out.flush(); in.readLine();
            } else if (choice == "3") return;
        }
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    CLIProject project;
    project.run();
    return a.exec();
}
