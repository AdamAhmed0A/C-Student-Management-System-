#ifndef PROFESSORPANEL_H
#define PROFESSORPANEL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QStackedWidget>

#include "../controllers/professorcontroller.h"
#include "../controllers/enrollmentcontroller.h"
#include "../controllers/coursecontroller.h"
#include "../controllers/schedulecontroller.h"
#include "../controllers/calendarcontroller.h"
#include "../controllers/academic_level_controller.h"

#include "../models/professor.h"
#include "../models/attendance_log.h"
#include <QDateEdit>

class ProfessorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ProfessorPanel(int userId, QWidget *parent = nullptr);
    ~ProfessorPanel();

private slots:
    void onCourseSelected(int index);
    void onYearSelected(int index);
    void onUpdateClicked(); // New slot for the top button
    void showGradeForm(int enrollmentId, int tabIndex = -1);
    void onSubmitAttendance();
    void onRefreshStudents(); // Refreshes both tables based on active tab
    void onRefreshAll();
    void onLogout();

    
    // Calendar Management
    void onAddCalendarEvent();
    void onEditCalendarEvent();
    void onDeleteCalendarEvent();

    // Profile
    void onSaveProfile();


private:
    void setupUI();
    void loadProfessorData();
    void loadCourses();
    void loadSchedule();
    void loadCalendarEvents();
    
    // Helpers
    QWidget* createAs1Tab();
    QWidget* createAs2Tab();
    QWidget* createCWTab();
    QWidget* createFinalTab();
    QWidget* createExpTab();
    QWidget* createEvaluationTab();
    QWidget* createAttendanceTab();
    QWidget* createScheduleTab();
    QWidget* createCoursesTab();
    QWidget* createProfileTab();
    QWidget* createCalendarTab();

    int m_userId;
    Professor m_professor;
    
    QTabWidget* m_tabWidget;
    
    // Shared Selectors
    QComboBox* m_yearSelector;
    QComboBox* m_courseSelector;

    // Separate Grade Tables
    QTableWidget* m_as1Table;
    QTableWidget* m_as2Table;
    QTableWidget* m_cwTable;
    QTableWidget* m_finalTable;
    QTableWidget* m_expTable;
    QTableWidget* m_evaluationTable;
    
    QLabel* m_noStudentsLabelGrades; // Shared or specific? I'll keep one or add more if needed.

    // Attendance
    QDateEdit* m_attendanceDate;
    QTableWidget* m_attendanceTable;
    QLabel* m_noStudentsLabelAtt;
    
    // Schedule
    QTableWidget* m_scheduleTable;
    
    // Courses
    QTableWidget* m_coursesTable;
    
    // Profile
    QLineEdit* m_profSpecializationEdit;
    QTextEdit* m_profInfoEdit;
    
    // Calendar
    QTableWidget* m_calendarTable;
    
    QList<Course> m_allProfessorCourses;
    
    ProfessorController m_professorController;
    EnrollmentController m_enrollmentController;
    CourseController m_courseController;
    ScheduleController m_scheduleController;
    CalendarController m_calendarController;
    AcademicLevelController m_academicLevelController;
};

#endif // PROFESSORPANEL_H
