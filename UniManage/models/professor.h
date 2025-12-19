#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <QString>

class Professor
{
public:
    Professor();
    Professor(int id, int userId, const QString& specialization, const QString& title);

    int id() const;
    void setId(int id);

    int userId() const;
    void setUserId(int userId);

    QString specialization() const;
    void setSpecialization(const QString& specialization);

    QString title() const;
    void setTitle(const QString& title);

    QString personalInfo() const;
    void setPersonalInfo(const QString& info);

    QString idNumber() const;
    void setIdNumber(const QString& idNumber);

    // Join helper
    QString fullName() const;
    void setFullName(const QString& name);

private:
    int m_id;
    int m_userId;
    QString m_specialization;
    QString m_title;
    QString m_personalInfo;
    QString m_idNumber;
    QString m_fullName;
};

#endif // PROFESSOR_H
