#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QDateTime>

class Course
{
public:
    Course();
    Course(int id, const QString& name, const QString& description, int yearLevel, int creditHours, int semesterId);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QString description() const;
    void setDescription(const QString& description);

    int yearLevel() const;
    void setYearLevel(int yearLevel);

    int creditHours() const;
    void setCreditHours(int creditHours);

    int semesterId() const;
    void setSemesterId(int semesterId);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime& updatedAt);

    // Helper fields
    QString semesterName() const; // e.g. "2024 - Sem 1"
    void setSemesterName(const QString& name);

private:
    int m_id;
    QString m_name;
    QString m_description;
    int m_yearLevel;
    int m_creditHours;
    int m_semesterId;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    QString m_semesterName;
};

#endif // COURSE_H
