#pragma once

#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>
#include "../controllers/studentcontroller.h"
#include "../models/student_data.h"

class StudentPortal : public QWidget
{
    Q_OBJECT

public:
    explicit StudentPortal(int studentId, QWidget *parent = nullptr);
    ~StudentPortal();

private:
    void setupUI();
    QWidget* createDashboardTab();
    QWidget* createCoursesTab();
    
    void loadDashboard();
    void loadCourses();

private:
    int m_studentId;
    StudentData m_student;
    
    QTabWidget* m_tabWidget;
    QLabel* m_studentNameLabel;
    QLabel* m_studentNumberLabel;
    QTableWidget* m_coursesTable;
    
    StudentController m_studentController;
};

#endif // STUDENTPORTAL_H
