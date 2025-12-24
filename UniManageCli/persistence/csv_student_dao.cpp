#include <QStringConverter>
#include "csv_student_dao.h"
#include <QTextStream>
#include <algorithm>

CsvStudentDAO::CsvStudentDAO(const QString& filePath)
    : m_filePath(filePath), m_nextId(1)
{
    ensureFileExists();
    loadData();
}

bool CsvStudentDAO::insert(StudentData& student)
{
    student.setId(m_nextId++);
    student.setCreatedAt(QDateTime::currentDateTime());
    student.setUpdatedAt(QDateTime::currentDateTime());
    m_students.append(student);
    return saveData();
}

bool CsvStudentDAO::update(const StudentData& student)
{
    for (auto& s : m_students) {
        if (s.id() == student.id()) {
            s = student;
            s.setUpdatedAt(QDateTime::currentDateTime());
            return saveData();
        }
    }
    return false;
}

bool CsvStudentDAO::remove(int id)
{
    auto it = std::remove_if(m_students.begin(), m_students.end(),
                             [id](const StudentData& s) { return s.id() == id; });
    if (it != m_students.end()) {
        m_students.erase(it, m_students.end());
        return saveData();
    }
    return false;
}

bool CsvStudentDAO::softDelete(int id)
{
    for (auto& s : m_students) {
        if (s.id() == id) {
            s.setStatus("deleted");
            s.setUpdatedAt(QDateTime::currentDateTime());
            return saveData();
        }
    }
    return false;
}

bool CsvStudentDAO::restore(int id)
{
    for (auto& s : m_students) {
        if (s.id() == id) {
            s.setStatus("active");
            s.setUpdatedAt(QDateTime::currentDateTime());
            return saveData();
        }
    }
    return false;
}

QList<StudentData> CsvStudentDAO::getAll()
{
    QList<StudentData> active;
    for (const auto& s : m_students) {
        if (s.status() != "deleted") {
            active.append(s);
        }
    }
    return active;
}

QList<StudentData> CsvStudentDAO::getDeleted()
{
    QList<StudentData> deleted;
    for (const auto& s : m_students) {
        if (s.status() == "deleted") {
            deleted.append(s);
        }
    }
    return deleted;
}

StudentData CsvStudentDAO::getById(int id)
{
    for (const auto& s : m_students) {
        if (s.id() == id) {
            return s;
        }
    }
    return StudentData();
}

StudentData CsvStudentDAO::getByUserId(int userId)
{
    for (const auto& s : m_students) {
        if (s.userId() == userId) {
            return s;
        }
    }
    return StudentData();
}

StudentData CsvStudentDAO::getByIdNumber(const QString& idNumber)
{
    for (const auto& s : m_students) {
        if (s.idNumber() == idNumber) {
            return s;
        }
    }
    return StudentData();
}

void CsvStudentDAO::loadData()
{
    m_students.clear();
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << m_filePath;
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // Read header
    if (!in.atEnd()) {
        QString header = in.readLine();
        QStringList headers = header.split(',');
        QStringList expected = getExpectedHeaders();
        if (headers != expected) {
            qWarning() << "CSV headers mismatch. Expected:" << expected << "Got:" << headers;
        }
    }

    // Read data
    while (!in.atEnd()) {
        QString line = in.readLine();
        line = line.trimmed();
        if (!line.isEmpty()) {
            StudentData student = parseLine(line);
            if (student.id() > 0) {
                m_students.append(student);
                m_nextId = std::max(m_nextId, student.id() + 1);
            }
        }
    }
    file.close();
}

bool CsvStudentDAO::saveData()
{
    QString tempPath = m_filePath + ".tmp";
    QFile file(tempPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open temp file:" << tempPath;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Write header
    out << getExpectedHeaders().join(',') << '\n';

    // Write data
    for (const auto& student : m_students) {
        out << studentToCsvLine(student) << '\n';
    }

    file.close();

    // Atomic rename
    if (QFile::exists(m_filePath)) {
        QFile::remove(m_filePath);
    }
    return QFile::rename(tempPath, m_filePath);
}

bool CsvStudentDAO::ensureFileExists()
{
    if (QFile::exists(m_filePath)) {
        return true;
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << getExpectedHeaders().join(',') << '\n';
    file.close();
    return true;
}

QStringList CsvStudentDAO::getExpectedHeaders() const
{
    return {"id", "user_id", "student_number", "id_number", "dob", "department", "department_id", "academic_level_id", "section_id", "college_id", "tuition_fees", "seat_number", "status", "created_at", "updated_at"};
}

StudentData CsvStudentDAO::parseLine(const QString& line) const
{
    QStringList parts = line.split(',');
    if (parts.size() < 15) return StudentData();

    StudentData student;
    student.setId(parts.at(0).toInt());
    student.setUserId(parts.at(1).toInt());
    student.setStudentNumber(parts.at(2));
    student.setIdNumber(parts.at(3));
    student.setDob(parseDateTime(parts.at(4)));
    student.setDepartment(parts.at(5));
    student.setDepartmentId(parts.at(6).toInt());
    student.setAcademicLevelId(parts.at(7).toInt());
    student.setSectionId(parts.at(8).toInt());
    student.setCollegeId(parts.at(9).toInt());
    student.setTuitionFees(parts.at(10).toDouble());
    student.setSeatNumber(parts.at(11));
    student.setStatus(parts.at(12));
    student.setCreatedAt(parseDateTime(parts.at(13)));
    student.setUpdatedAt(parseDateTime(parts.at(14)));
    return student;
}

QString CsvStudentDAO::studentToCsvLine(const StudentData& student) const
{
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15")
        .arg(student.id())
        .arg(student.userId())
        .arg(student.studentNumber())
        .arg(student.idNumber())
        .arg(dateTimeToString(student.dob()))
        .arg(student.department())
        .arg(student.departmentId())
        .arg(student.academicLevelId())
        .arg(student.sectionId())
        .arg(student.collegeId())
        .arg(student.tuitionFees())
        .arg(student.seatNumber())
        .arg(student.status())
        .arg(dateTimeToString(student.createdAt()))
        .arg(dateTimeToString(student.updatedAt()));
}

QDateTime CsvStudentDAO::parseDateTime(const QString& str) const
{
    QDate date = QDate::fromString(str, Qt::ISODate);
    if (date.isValid()) {
        return QDateTime(date, QTime());
    }
    return QDateTime::fromString(str, Qt::ISODate);
}

QString CsvStudentDAO::dateTimeToString(const QDateTime& dt) const
{
    return dt.date().toString(Qt::ISODate);
}