#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QDateTime>

class Section
{
public:
    Section();
    Section(int id, int courseId, int capacity, int semesterId);

    int id() const;
    void setId(int id);

    int courseId() const;
    void setCourseId(int courseId);

    int capacity() const;
    void setCapacity(int capacity);

    int semesterId() const;
    void setSemesterId(int semesterId);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime& updatedAt);

    // Helpers
    QString courseName() const;
    void setCourseName(const QString& courseName);

private:
    int m_id;
    int m_courseId;
    int m_capacity;
    int m_semesterId;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    QString m_courseName;
};

#endif // SECTION_H
