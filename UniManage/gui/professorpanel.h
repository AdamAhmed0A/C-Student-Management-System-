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

#include "../controllers/professorcontroller.h"
#include "../controllers/enrollmentcontroller.h"
#include "../controllers/coursecontroller.h"
#include "../controllers/schedulecontroller.h"
#include "../controllers/newscontroller.h"

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
    void onSaveGrades();
    void onTakeAttendance();
    void onRefreshStudents();
    void onRefreshAll();
    void onLogout();
    
    // Course Management
    void onAddCourse();
    void onEditCourse();
    void onDeleteCourse();

    // Profile
    void onSaveProfile();

private:
    void setupUI();
    void loadProfessorData();
    void loadCourses();
    void loadSchedule();
    void loadNews();
    
    // Helpers
    QWidget* createGradingTab();
    QWidget* createScheduleTab();
    QWidget* createCoursesTab();
    QWidget* createProfileTab();
    QWidget* createCalendarTab();

    int m_userId;
    Professor m_professor;
    
    QTabWidget* m_tabWidget;
    
    // Grading
    QComboBox* m_courseSelector;
    QTableWidget* m_studentsTable;
    
    // Schedule
    QTableWidget* m_scheduleTable;
    
    // Courses
    QTableWidget* m_coursesTable;
    
    // Profile
    QLineEdit* m_profEmailEdit; // Username
    QLineEdit* m_profSpecializationEdit;
    QLineEdit* m_profTitleEdit;
    QTextEdit* m_profInfoEdit;
    
    // Calendar
    QListWidget* m_newsList; // Using ListWidget for news items
    
    ProfessorController m_professorController;
    EnrollmentController m_enrollmentController;
    CourseController m_courseController;
    ScheduleController m_scheduleController;
    // We'll init NewsController inside logic if needed or add member
};

#endif // PROFESSORPANEL_H
