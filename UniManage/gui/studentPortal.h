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
#include "../controllers/academic_level_controller.h"
#include "../controllers/schedulecontroller.h"
#include "../controllers/collegecontroller.h"
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
    void refreshSchedule();
    void refreshCalendar();
    void calculateGPA();
    
private slots:
    void onLogout();
    void onRefreshAll();
    void onMakePayment();

private:
    int m_userId;
    StudentData m_student;
    
    QTabWidget* m_tabWidget;
    QLabel* m_nameLabel;
    QLabel* m_gpaLabel;
    QLabel* m_tuitionLabel;
    QLabel* m_paidLabel;
    QLabel* m_balanceLabel;
    QTableWidget* m_gradesTable;
    QTableWidget* m_scheduleTable;
    QTableWidget* m_paymentsTable;
    
    StudentController m_studentController;
    EnrollmentController m_enrollmentController;
    PaymentController m_paymentController;
    CalendarController m_calendarController;
    AcademicLevelController m_academicLevelController;
    ScheduleController m_scheduleController;
    CollegeController m_collegeController;
    QTableWidget* m_calendarTable;
};

#endif // STUDENTPORTAL_H
