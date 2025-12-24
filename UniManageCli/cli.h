#ifndef CLI_H
#define CLI_H

#include <QObject>
#include "clicontroller.h"

class Cli : public QObject
{
    Q_OBJECT
public:
    explicit Cli(QObject *parent = nullptr);
    void run();
    bool login();

private:
    void displayMainMenu();
    void displayStudentMenu();
    void displayCourseMenu();
    void displayProfessorMenu();
    void displayUserMenu();

    // Student functions
    void listAllStudents();
    void findStudentById();
    void addStudent();
    void updateStudent();
    void deleteStudent();

    // Course functions
    void listAllCourses();
    void findCourseById();
    void addCourse();
    void updateCourse();
    void deleteCourse();

    // Professor functions
    void listAllProfessors();
    void findProfessorById();
    void addProfessor();
    void updateProfessor();
    void deleteProfessor();

    // User functions
    void listAllUsers();
    void addUser();
    void deleteUser();

    CliController m_controller;
};

#endif // CLI_H
