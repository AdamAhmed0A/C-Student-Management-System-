#ifndef COURSE_DAO_H
#define COURSE_DAO_H

#include "../UniManage/models/course.h"
#include <QList>
#include <QString>

class CourseDAO
{
public:
    virtual ~CourseDAO() = default;

    virtual bool insert(Course& course) = 0;
    virtual bool update(const Course& course) = 0;
    virtual bool remove(int id) = 0;
    virtual QList<Course> getAll() = 0;
    virtual Course getById(int id) = 0;
    virtual QList<Course> getBySemester(int semesterId) = 0;
};

#endif // COURSE_DAO_H