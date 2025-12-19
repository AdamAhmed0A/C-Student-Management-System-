#ifndef COURSECONTROLLER_H
#define COURSECONTROLLER_H

#include "../models/course.h"
#include <QList>

class CourseController
{
public:
    CourseController();

    bool addCourse(const Course& course);
    bool updateCourse(const Course& course);
    bool deleteCourse(int id);

    QList<Course> getAllCourses();
    QList<Course> getCoursesBySemester(int semesterId);
};

#endif // COURSECONTROLLER_H
