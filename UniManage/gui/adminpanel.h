#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include "controllers/studentcontroller.h"
#include "controllers/coursecontroller.h"
#include "controllers/usercontroller.h"
#include "controllers/semestercontroller.h"
#include "controllers/collegecontroller.h"
#include "controllers/departmentcontroller.h"
#include "controllers/academic_level_controller.h"
#include "controllers/roomcontroller.h"
#include <QComboBox>
#include "controllers/professorcontroller.h"
#include "controllers/calendarcontroller.h"
#include "controllers/schedulecontroller.h"
#include "controllers/sectioncontroller.h"
#include "models/semester.h"

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
    QWidget* createFacultiesTab();
    QWidget* createDepartmentsTab();
    QWidget* createRoomsTab();
    QWidget* createProfessorsTab();
    QWidget* createSchedulesTab();
    QWidget* createAcademicSetupTab();
    QWidget* createSectionsTab();
    QWidget* createCalendarTab();
    QWidget* createDraftTab();
    
    void refreshStudentsTable();
    void refreshCoursesTable();
    void refreshCollegesTable();
    void refreshDepartmentsTable();
    void refreshLevelsTable();
    void refreshRoomsTable();
    void refreshProfessorsTable();
    void refreshSchedulesTable();
    void refreshSectionsTable();
    void refreshCalendarTable();
    void refreshDraftTable();
    void refreshAllData();

private slots:
    void onLogout();
    void onRefreshAll();
    void onTestDatabase();
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onAddCourse();
    void onEditCourse();
    void onDeleteCourse();
    
    // Setup slots
    void onAddCollege();
    void onAddDepartment();
    void onAddLevel();
    void onAddRoom();
    void onAddProfessor();
    void onEditProfessor();
    void onDeleteProfessor();
    void onAddSchedule();
    void onManageRoomSpecs();
    void onAssignProfessor();
    void onAddCalendarEvent();
    void onEditCalendarEvent();
    void onDeleteCalendarEvent();
    void onEditCollege();
    void onDeleteCollege();
    void onEditRoom();
    void onDeleteRoom();
    void onEditDepartment();
    void onDeleteDepartment();
    void onPrintData();
    void onEditLevel();
    void onDeleteLevel();
    void onAddSection(); 
    void onEditSection();
    void onDeleteSection();
    void onEditSchedule();
    void onDeleteSchedule();
    void onRestoreStudent();
    void onHardDeleteStudent();

private:
    int m_adminId;
    QTabWidget* m_tabWidget;
    QTabWidget* m_roomSubTabWidget;
    QTableWidget* m_studentsTable;
    QTableWidget* m_coursesTable;
    QTableWidget* m_collegesTable;
    QTableWidget* m_departmentsTable;
    QTableWidget* m_levelsTable;
    QTableWidget* m_roomsTable;
    QTableWidget* m_labsTable;
    QTableWidget* m_professorsTable;
    QTableWidget* m_schedulesTable;
    
    StudentController m_studentController;
    CourseController m_courseController;
    UserController m_userController;
    SemesterController m_semesterController;
    CollegeController m_collegeController;
    DepartmentController m_departmentController;
    AcademicLevelController m_academicLevelController;
    RoomController m_roomController;
    ProfessorController m_professorController;
    CalendarController m_calendarController;
    ScheduleController m_scheduleController;
    SectionController m_sectionController;
    QTableWidget* m_calendarTable;
    QTableWidget* m_sectionsTable;
    QTableWidget* m_draftStudentsTable;
};

#endif // ADMINPANEL_H
