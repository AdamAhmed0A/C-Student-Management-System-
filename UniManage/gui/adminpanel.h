#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include "../controllers/studentcontroller.h"
#include "../controllers/coursecontroller.h"
#include "../controllers/usercontroller.h"

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

private:
    int m_adminId;
    QTabWidget* m_tabWidget;
    QTableWidget* m_studentsTable;
    QTableWidget* m_coursesTable;
    
    StudentController m_studentController;
    CourseController m_courseController;
    UserController m_userController;
};

#endif // ADMINPANEL_H
