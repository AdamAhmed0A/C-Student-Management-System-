#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>
#include "../controllers/studentcontroller.h"
#include "../controllers/enrollmentcontroller.h"
#include "../controllers/paymentcontroller.h"
#include "../controllers/calendarcontroller.h"
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
    QWidget* createCalendarTab();
    
    void loadStudentData();
    void refreshGrades();
    void refreshPayments();
    void refreshCalendar();
    void calculateGPA();
    
private slots:
    void onLogout();
    void onRefreshAll();

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
    CalendarController m_calendarController;
    QTableWidget* m_calendarTable;
};

#endif // STUDENTPORTAL_H
