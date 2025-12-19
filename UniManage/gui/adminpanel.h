#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include "../controllers/studentcontroller.h"
#include "../controllers/coursecontroller.h"
#include "../controllers/usercontroller.h"
#include "../controllers/semestercontroller.h"
#include "../controllers/collegecontroller.h"
#include "../controllers/departmentcontroller.h"
#include "../controllers/academic_level_controller.h"
#include "../controllers/roomcontroller.h"
#include "../controllers/professorcontroller.h"
#include "../models/semester.h"

class AdminPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanel(int adminId, QWidget *parent = nullptr);
    ~AdminPanel();

private:
    void setupUI();
    QWidget* createStudentsTab();
    QWidget* createCoursesTab();
    QWidget* createSetupTab();
    
    void refreshStudentsTable();
    void refreshCoursesTable();

private slots:
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onAddCourse();
    // New slots
    void onEditCourse();
    void onDeleteCourse();
    
    // Setup slots
    void onAddCollege();
    void onAddDepartment();
    void onAddLevel();
    void onAddRoom();
    void onAddProfessor();

private:
    int m_adminId;
    QTabWidget* m_tabWidget;
    QTableWidget* m_studentsTable;
    QTableWidget* m_coursesTable;
    QTableWidget* m_collegesTable;
    QTableWidget* m_departmentsTable;
    QTableWidget* m_levelsTable;
    QTableWidget* m_roomsTable;
    QTableWidget* m_professorsTable;
    
    StudentController m_studentController;
    CourseController m_courseController;
    UserController m_userController;
    SemesterController m_semesterController;
    CollegeController m_collegeController;
    DepartmentController m_departmentController;
    AcademicLevelController m_academicLevelController;
    RoomController m_roomController;
    ProfessorController m_professorController;
};

#endif // ADMINPANEL_H
