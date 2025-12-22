#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <QString>
#include <QDateTime>

class Enrollment
{
public:
    Enrollment();

    int id() const;
    void setId(int id);

    int studentId() const;
    void setStudentId(int studentId);

    int courseId() const;
    void setCourseId(int courseId);

    QString status() const;
    void setStatus(const QString& status);

    int attendanceCount() const;
    void setAttendanceCount(int count);

    int absenceCount() const;
    void setAbsenceCount(int count);

    double assignment1Grade() const;
    void setAssignment1Grade(double grade);

    double assignment2Grade() const;
    void setAssignment2Grade(double grade);

    double courseworkGrade() const;
    void setCourseworkGrade(double grade);

    double finalExamGrade() const;
    void setFinalExamGrade(double grade);

    double totalGrade() const;
    void setTotalGrade(double grade);

    QString letterGrade() const;
    void setLetterGrade(const QString& grade);

    bool isRafaaApplied() const;
    void setIsRafaaApplied(bool applied);

    QDateTime enrolledAt() const;
    void setEnrolledAt(const QDateTime& enrolledAt);

    // Helper fields for UI
    QString studentName() const;
    void setStudentName(const QString& name);
    
    QString courseName() const;
    void setCourseName(const QString& name);
    
    int courseMaxGrade() const;
    void setCourseMaxGrade(int maxGrade);
    
    QString courseType() const;
    void setCourseType(const QString& type);
    
    QString studentSection() const;
    void setStudentSection(const QString& section);

    QString studentLevel() const;
    void setStudentLevel(const QString& level);

    QString studentCode() const;
    void setStudentCode(const QString& code);

    QString academicYear() const;
    void setAcademicYear(const QString& year);

private:
    int m_id;
    int m_studentId;
    int m_courseId;
    QString m_status;
    int m_attendanceCount;
    int m_absenceCount;
    double m_assignment1Grade;
    double m_assignment2Grade;
    double m_courseworkGrade;
    double m_finalExamGrade;
    double m_totalGrade;
    QString m_letterGrade;
    bool m_isRafaaApplied;
    QDateTime m_enrolledAt;
    
    QString m_studentName;
    QString m_courseName;
    int m_courseMaxGrade;
    QString m_courseType;
    QString m_studentSection;
    QString m_studentLevel;
    QString m_studentCode;
    QString m_academicYear;
};

#endif // ENROLLMENT_H
