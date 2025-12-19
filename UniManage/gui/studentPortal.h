#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>
#include "../controllers/studentcontroller.h"
#include "../controllers/enrollmentcontroller.h"
#include "../controllers/paymentcontroller.h"
#include "../models/student_data.h"

class StudentPortal : public QWidget
{
    Q_OBJECT

public:
    explicit StudentPortal(int studentUserId, QWidget *parent = nullptr);
    ~StudentPortal();

private:
    void setupUI();
    QWidget* createDashboardTab();
    QWidget* createGradesTab();
    QWidget* createScheduleTab();
    QWidget* createPaymentsTab();
    
    void loadStudentData();
    void refreshGrades();
    void refreshPayments();
    void calculateGPA();
    
private slots:
    void onLogout();

private:
    int m_userId;
    StudentData m_student;
    
    QTabWidget* m_tabWidget;
    QLabel* m_nameLabel;
    QLabel* m_gpaLabel;
    QTableWidget* m_gradesTable;
    QTableWidget* m_scheduleTable;
    QTableWidget* m_paymentsTable;
    
    StudentController m_studentController;
    EnrollmentController m_enrollmentController;
    PaymentController m_paymentController;
};

#endif // STUDENTPORTAL_H
