#include "csv_course_dao.h"
#include <QtCore>
#include <algorithm>

CsvCourseDAO::CsvCourseDAO(const QString& filePath)
    : m_filePath(filePath), m_nextId(1)
{
    ensureFileExists();
    loadData();
}

bool CsvCourseDAO::insert(Course& course)
{
    if (course.id() <= 0) {
        course.setId(m_nextId++);
        course.setCreatedAt(QDateTime::currentDateTime());
    }
    course.setUpdatedAt(QDateTime::currentDateTime());

    m_courses.append(course);
    return saveData();
}

bool CsvCourseDAO::update(const Course& course)
{
    for (auto& c : m_courses) {
        if (c.id() == course.id()) {
            c = course;
            c.setUpdatedAt(QDateTime::currentDateTime());
            return saveData();
        }
    }
    return false;
}

bool CsvCourseDAO::remove(int id)
{
    auto it = std::remove_if(m_courses.begin(), m_courses.end(),
                             [id](const Course& c) { return c.id() == id; });
    if (it != m_courses.end()) {
        m_courses.erase(it, m_courses.end());
        return saveData();
    }
    return false;
}

QList<Course> CsvCourseDAO::getAll()
{
    return m_courses;
}

Course CsvCourseDAO::getById(int id)
{
    for (const auto& c : m_courses) {
        if (c.id() == id) {
            return c;
        }
    }
    return Course(); // Return default course if not found
}

QList<Course> CsvCourseDAO::getBySemester(int semesterId)
{
    QList<Course> result;
    for (const auto& c : m_courses) {
        if (c.semesterId() == semesterId) {
            result.append(c);
        }
    }
    return result;
}

void CsvCourseDAO::loadData()
{
    m_courses.clear();
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << m_filePath;
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine(); // Skip header

    while (!in.atEnd()) {
        line = in.readLine();
        if (!line.isEmpty()) {
            Course course = parseLine(line);
            if (course.id() > 0) {
                m_courses.append(course);
                m_nextId = std::max(m_nextId, course.id() + 1);
            }
        }
    }
}

bool CsvCourseDAO::saveData()
{
    QString tempPath = m_filePath + ".tmp";
    QFile file(tempPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open temp file for writing:" << tempPath;
        return false;
    }

    QTextStream out(&file);
    out << getExpectedHeaders().join(',') << '\n';

    for (const auto& course : m_courses) {
        out << courseToCsvLine(course) << '\n';
    }

    file.close();

    if (QFile::exists(m_filePath)) {
        QFile::remove(m_filePath);
    }
    return QFile::rename(tempPath, m_filePath);
}

bool CsvCourseDAO::ensureFileExists()
{
    if (QFile::exists(m_filePath)) {
        return true;
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to create file:" << m_filePath;
        return false;
    }

    QTextStream out(&file);
    out << getExpectedHeaders().join(',') << '\n';
    return true;
}

QStringList CsvCourseDAO::getExpectedHeaders() const
{
    return QStringList()
        << "id"
        << "name"
        << "description"
        << "year_level"
        << "credit_hours"
        << "semester_id"
        << "department_id"
        << "max_grade"
        << "course_type"
        << "created_at"
        << "updated_at";
}

Course CsvCourseDAO::parseLine(const QString& line) const
{
    QStringList parts = line.split(',');
    if (parts.size() < 11) {
        return Course();
    }

    Course course;
    course.setId(parts[0].toInt());
    course.setName(parts[1]);
    course.setDescription(parts[2]);
    course.setYearLevel(parts[3].toInt());
    course.setCreditHours(parts[4].toInt());
    course.setSemesterId(parts[5].toInt());
    course.setDepartmentId(parts[6].toInt());
    course.setMaxGrade(parts[7].toInt());
    course.setCourseType(parts[8]);
    course.setCreatedAt(parseDateTime(parts[9]));
    course.setUpdatedAt(parseDateTime(parts[10]));

    return course;
}

QString CsvCourseDAO::courseToCsvLine(const Course& course) const
{
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
        .arg(course.id())
        .arg(course.name())
        .arg(course.description())
        .arg(course.yearLevel())
        .arg(course.creditHours())
        .arg(course.semesterId())
        .arg(course.departmentId())
        .arg(course.maxGrade())
        .arg(course.courseType())
        .arg(dateTimeToString(course.createdAt()))
        .arg(dateTimeToString(course.updatedAt()));
}

QDateTime CsvCourseDAO::parseDateTime(const QString& str) const
{
    return QDateTime::fromString(str, Qt::ISODate);
}

QString CsvCourseDAO::dateTimeToString(const QDateTime& dt) const
{
    return dt.toString(Qt::ISODate);
}