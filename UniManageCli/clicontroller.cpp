#include "clicontroller.h"
#include <QCoreApplication>

CliController::CliController()
{
    QString basePath = QCoreApplication::applicationDirPath() + "/FileDB";
    m_userDAO = std::make_unique<CsvUserDAO>(basePath + "/users.csv");
    m_studentDAO = std::make_unique<CsvStudentDAO>(basePath + "/students_data.csv");
    m_courseDAO = std::make_unique<CsvCourseDAO>(basePath + "/courses.csv");
    m_professorDAO = std::make_unique<CsvProfessorDAO>(basePath + "/professors.csv");

    // Initialize default admin user if no users exist
    if (m_userDAO->getAll().isEmpty()) {
        User admin;
        admin.setFullName("Administrator");
        admin.setUsername("admin");
        admin.setPassword("12345678901234"); // Example 14-digit National ID
        admin.setRole("admin");
        m_userDAO->insert(admin);
    }
}

CliController::~CliController() = default;

// --- User Functions ---

User CliController::authenticate(const QString& username, const QString& password)
{
    return m_userDAO->authenticate(username, password);
}

bool CliController::addUser(User& user)
{
    return m_userDAO->insert(user);
}

bool CliController::updateUser(const User& user)
{
    return m_userDAO->update(user);
}

bool CliController::deleteUser(int id)
{
    return m_userDAO->remove(id);
}

QList<User> CliController::getAllUsers()
{
    return m_userDAO->getAll();
}

User CliController::getUserById(int id)
{
    return m_userDAO->getById(id);
}

User CliController::getUserByUsername(const QString& username)
{
    return m_userDAO->getByUsername(username);
}

// --- Student Functions ---

QList<StudentData> CliController::getAllStudents()
{
    QList<StudentData> students = m_studentDAO->getAll();
    for (auto& student : students) {
        User user = getUserById(student.userId());
        student.setFullName(user.fullName());
    }
    return students;
}

StudentData CliController::getStudentById(int id)
{
    StudentData student = m_studentDAO->getById(id);
    if (student.id() != 0) {
        User user = getUserById(student.userId());
        student.setFullName(user.fullName());
    }
    return student;
}

bool CliController::addStudent(StudentData& student)
{
    return m_studentDAO->insert(student);
}

bool CliController::updateStudent(const StudentData& student)
{
    return m_studentDAO->update(student);
}

bool CliController::deleteStudent(int id)
{
    return m_studentDAO->softDelete(id); // or remove, but soft delete to match GUI
}

StudentData CliController::getStudentByUserId(int userId)
{
    StudentData student = m_studentDAO->getByUserId(userId);
    if (student.id() != 0) {
        User user = getUserById(student.userId());
        student.setFullName(user.fullName());
    }
    return student;
}

StudentData CliController::getStudentByStudentNumber(const QString& studentNumber)
{
    StudentData student = m_studentDAO->getByStudentNumber(studentNumber);
    if (student.id() != 0) {
        User user = getUserById(student.userId());
        student.setFullName(user.fullName());
    }
    return student;
}

// --- Course Functions ---

QList<Course> CliController::getAllCourses()
{
    return m_courseDAO->getAll();
}

Course CliController::getCourseById(int id)
{
    return m_courseDAO->getById(id);
}

bool CliController::addCourse(Course& course)
{
    return m_courseDAO->insert(course);
}

bool CliController::updateCourse(const Course& course)
{
    return m_courseDAO->update(course);
}

bool CliController::deleteCourse(int id)
{
    return m_courseDAO->remove(id);
}

// --- Professor Functions ---

QList<Professor> CliController::getAllProfessors()
{
    QList<Professor> professors = m_professorDAO->getAll();
    for (auto& prof : professors) {
        User user = getUserById(prof.userId());
        prof.setFullName(user.fullName());
        prof.setUsername(user.username());
    }
    return professors;
}

Professor CliController::getProfessorById(int id)
{
    return m_professorDAO->getById(id);
}

bool CliController::addProfessor(Professor& professor)
{
    return m_professorDAO->insert(professor);
}

bool CliController::updateProfessor(const Professor& professor)
{
    return m_professorDAO->update(professor);
}

bool CliController::deleteProfessor(int id)
{
    return m_professorDAO->remove(id);
}

Professor CliController::getProfessorByUserId(int userId)
{
    return m_professorDAO->getByUserId(userId);
}

Professor CliController::getProfessorByUsername(const QString& username)
{
    User u = getUserByUsername(username);
    if (u.id() == 0) return Professor();
    Professor p = getProfessorByUserId(u.id());
    p.setFullName(u.fullName());
    p.setUsername(u.username());
    return p;
}
