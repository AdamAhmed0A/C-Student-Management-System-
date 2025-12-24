#ifndef CSV_COURSE_DAO_H
#define CSV_COURSE_DAO_H

#include "dao/course_dao.h"
#include "../../UniManage/models/course.h"
#include <QList>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

class CsvCourseDAO : public CourseDAO
{
public:
    CsvCourseDAO(const QString& filePath);
    ~CsvCourseDAO() override = default;

    bool insert(Course& course) override;
    bool update(const Course& course) override;
    bool remove(int id) override;
    QList<Course> getAll() override;
    Course getById(int id) override;
    QList<Course> getBySemester(int semesterId) override;

private:
    QString m_filePath;
    QList<Course> m_courses;
    int m_nextId;

    void loadData();
    bool saveData();
    bool ensureFileExists();
    QStringList getExpectedHeaders() const;
    Course parseLine(const QString& line) const;
    QString courseToCsvLine(const Course& course) const;
    QDateTime parseDateTime(const QString& str) const;
    QString dateTimeToString(const QDateTime& dt) const;
};

#endif // CSV_COURSE_DAO_H