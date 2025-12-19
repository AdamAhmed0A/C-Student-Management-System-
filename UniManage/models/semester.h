#ifndef SEMESTER_H
#define SEMESTER_H

#include <QDateTime>

class Semester
{
public:
    Semester();
    Semester(int id, const QDateTime& year, int semester);

    int id() const;
    void setId(int id);

    QDateTime year() const;
    void setYear(const QDateTime& year);

    int semester() const;
    void setSemester(int semester);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime& updatedAt);

private:
    int m_id;
    QDateTime m_year;
    int m_semester;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
};

#endif // SEMESTER_H
