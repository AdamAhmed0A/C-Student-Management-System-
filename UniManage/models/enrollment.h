#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <QString>
#include <QDateTime>

class Enrollment
{
public:
    Enrollment();
    Enrollment(int id, int studentId, const QString& status, const QString& assessmentsGrades,
               const QString& assignment1, const QString& assignment2, const QString& finalGrade,
               int totalMarks, bool isRafaaApplied, const QDateTime& enrolledAt, const QDateTime& createdAt);

    int id() const;
    void setId(int id);

    int studentId() const;
    void setStudentId(int studentId);

    QString status() const;
    void setStatus(const QString& status);

    QString assessmentsGrades() const;
    void setAssessmentsGrades(const QString& grades);

    QString assignment1() const;
    void setAssignment1(const QString& val);

    QString assignment2() const;
    void setAssignment2(const QString& val);

    QString finalGrade() const;
    void setFinalGrade(const QString& val);

    int totalMarks() const;
    void setTotalMarks(int totalMarks);

    bool isRafaaApplied() const;
    void setIsRafaaApplied(bool applied);

    QDateTime enrolledAt() const;
    void setEnrolledAt(const QDateTime& enrolledAt);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

private:
    int m_id;
    int m_studentId;
    QString m_status;
    QString m_assessmentsGrades;
    QString m_assignment1;
    QString m_assignment2;
    QString m_final;
    int m_totalMarks;
    bool m_isRafaaApplied;
    QDateTime m_enrolledAt;
    QDateTime m_createdAt;
};

#endif // ENROLLMENT_H
