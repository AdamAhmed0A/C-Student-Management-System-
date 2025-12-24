#ifndef CLICONTROLLER_H
#define CLICONTROLLER_H

#include "../UniManage/models/user.h"
#include "../UniManage/models/student_data.h"
#include "../UniManage/models/course.h"
#include "../UniManage/models/professor.h"
#include "persistence/csv_user_dao.h"
#include "persistence/csv_student_dao.h"
#include "persistence/csv_course_dao.h"
#include "persistence/csv_professor_dao.h"
#include <QString>
#include <QList>
#include <QDateTime>
#include <memory>

class CsvUserDAO;
class CsvStudentDAO;
class CsvCourseDAO;
class CsvProfessorDAO;

class CliController
{
public:
    CliController();
    ~CliController();

    // User functions
    User authenticate(const QString& username, const QString& password);
    bool addUser(User& user);
    bool deleteUser(int id);
    QList<User> getAllUsers();
    User getUserById(int id);

    // Student functions
    QList<StudentData> getAllStudents();
    StudentData getStudentById(int id);
    bool addStudent(StudentData& student);
    bool updateStudent(const StudentData& student);
    bool deleteStudent(int id);
    StudentData getStudentByUserId(int userId);

    // Course functions
    QList<Course> getAllCourses();
    Course getCourseById(int id);
    bool addCourse(Course& course);
    bool updateCourse(const Course& course);
    bool deleteCourse(int id);

    // Professor functions
    QList<Professor> getAllProfessors();
    Professor getProfessorById(int id);
    bool addProfessor(Professor& professor);
    bool updateProfessor(const Professor& professor);
    bool deleteProfessor(int id);
    Professor getProfessorByUserId(int userId);

private:
    std::unique_ptr<CsvUserDAO> m_userDAO;
    std::unique_ptr<CsvStudentDAO> m_studentDAO;
    std::unique_ptr<CsvCourseDAO> m_courseDAO;
    std::unique_ptr<CsvProfessorDAO> m_professorDAO;
};

#endif // CLICONTROLLER_H
