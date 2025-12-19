#ifndef PROFESSORPANEL_H
#define PROFESSORPANEL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include "../controllers/professorcontroller.h"
#include "../controllers/enrollmentcontroller.h"
#include "../controllers/coursecontroller.h"
#include "../models/professor.h"

class ProfessorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ProfessorPanel(int userId, QWidget *parent = nullptr);
    ~ProfessorPanel();

private slots:
    void onCourseSelected(int index);
    void onSaveGrades();
    void onRefreshStudents();
    void onRefreshAll();
    void onLogout();

private:
    void setupUI();
    void loadProfessorData();
    void loadCourses();

    int m_userId;
    Professor m_professor;
    
    QTabWidget* m_tabWidget;
    QComboBox* m_courseSelector;
    QTableWidget* m_studentsTable;
    QTableWidget* m_scheduleTable;
    
    ProfessorController m_professorController;
    EnrollmentController m_enrollmentController;
    CourseController m_courseController;
};

#endif // PROFESSORPANEL_H
