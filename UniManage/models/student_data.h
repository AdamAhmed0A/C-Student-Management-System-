#ifndef STUDENT_DATA_H
#define STUDENT_DATA_H

#include <QString>
#include <QDateTime>

class StudentData
{
public:
    StudentData();
    StudentData(int id, int userId, const QString &studentNumber, const QString &idNumber, const QString &phone, const QDateTime &dob, int year, const QString &department, int sectionId, const QString &seatNumber, const QString &status);

    int id() const;
    void setId(int id);

    int userId() const;
    void setUserId(int userId);

    QString studentNumber() const;
    void setStudentNumber(const QString &studentNumber);

    QString idNumber() const;
    void setIdNumber(const QString &idNumber);

    QString phone() const;
    void setPhone(const QString &phone);

    QDateTime dob() const;
    void setDob(const QDateTime &dob);

    int year() const;
    void setYear(int year);

    QString department() const;
    void setDepartment(const QString &department);

    int sectionId() const;
    void setSectionId(int sectionId);

    QString seatNumber() const;
    void setSeatNumber(const QString &seatNumber);

    QString status() const;
    void setStatus(const QString &status);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &createdAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime &updatedAt);

    // Helper to store joined user name
    QString fullName() const;
    void setFullName(const QString &fullName);

    // Helper to store joined user username
    QString username() const;
    void setUsername(const QString &username);

private:
    int m_id;
    int m_userId;
    QString m_studentNumber;
    QString m_idNumber;
    QString m_phone;
    QDateTime m_dob;
    int m_year;
    QString m_department;
    int m_sectionId;
    QString m_seatNumber;
    QString m_status;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    QString m_fullName; // Joined from User
    QString m_username; // Joined from User
};

#endif // STUDENT_DATA_H
