#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <QString>

class Department
{
public:
    Department();
    Department(int id, const QString& name, int collegeId, const QString& code);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    int collegeId() const;
    void setCollegeId(int collegeId);

    QString code() const;
    void setCode(const QString& code);

    // Join helper
    QString collegeName() const;
    void setCollegeName(const QString& name);

private:
    int m_id;
    QString m_name;
    int m_collegeId;
    QString m_code;
    QString m_collegeName;
};

#endif // DEPARTMENT_H
