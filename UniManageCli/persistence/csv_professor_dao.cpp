#include <QStringConverter>
#include "csv_professor_dao.h"
#include <QTextStream>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>
#include <algorithm>

CsvProfessorDAO::CsvProfessorDAO(const QString& filePath)
    : m_filePath(filePath), m_nextId(1)
{
    ensureFileExists();
    loadData();
}

bool CsvProfessorDAO::insert(Professor& professor)
{
    professor.setId(m_nextId++);
    m_professors.append(professor);
    return saveData();
}

bool CsvProfessorDAO::update(const Professor& professor)
{
    for (auto& p : m_professors) {
        if (p.id() == professor.id()) {
            p = professor;
            return saveData();
        }
    }
    return false;
}

bool CsvProfessorDAO::remove(int id)
{
    auto it = std::remove_if(m_professors.begin(), m_professors.end(),
                             [id](const Professor& p) { return p.id() == id; });
    if (it != m_professors.end()) {
        m_professors.erase(it, m_professors.end());
        return saveData();
    }
    return false;
}

QList<Professor> CsvProfessorDAO::getAll()
{
    return m_professors;
}

Professor CsvProfessorDAO::getById(int id)
{
    for (const auto& p : m_professors) {
        if (p.id() == id) {
            return p;
        }
    }
    return Professor();
}

Professor CsvProfessorDAO::getByUserId(int userId)
{
    for (const auto& p : m_professors) {
        if (p.userId() == userId) {
            return p;
        }
    }
    return Professor();
}

Professor CsvProfessorDAO::getByIdNumber(const QString& idNumber)
{
    for (const auto& p : m_professors) {
        if (p.idNumber() == idNumber) {
            return p;
        }
    }
    return Professor();
}

void CsvProfessorDAO::loadData()
{
    m_professors.clear();
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
            Professor professor = parseLine(line);
            if (professor.id() > 0) {
                m_professors.append(professor);
                m_nextId = std::max(m_nextId, professor.id() + 1);
            }
        }
    }
    file.close();
}

bool CsvProfessorDAO::saveData()
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
    for (const auto& professor : m_professors) {
        out << professorToCsvLine(professor) << '\n';
    }

    file.close();

    // Atomic rename
    if (QFile::exists(m_filePath)) {
        QFile::remove(m_filePath);
    }
    return QFile::rename(tempPath, m_filePath);
}

bool CsvProfessorDAO::ensureFileExists()
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

QStringList CsvProfessorDAO::getExpectedHeaders() const
{
    return {"id", "user_id", "specialization", "title", "personal_info", "id_number", "full_name"};
}

Professor CsvProfessorDAO::parseLine(const QString& line) const
{
    QStringList parts = line.split(',');
    if (parts.size() < 7) return Professor();

    Professor professor;
    professor.setId(parts.at(0).toInt());
    professor.setUserId(parts.at(1).toInt());
    professor.setSpecialization(parts.at(2));
    professor.setTitle(parts.at(3));
    professor.setPersonalInfo(parts.at(4));
    professor.setIdNumber(parts.at(5));
    professor.setFullName(parts.at(6));
    return professor;
}

QString CsvProfessorDAO::professorToCsvLine(const Professor& professor) const
{
    return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(professor.id())
        .arg(professor.userId())
        .arg(professor.specialization())
        .arg(professor.title())
        .arg(professor.personalInfo())
        .arg(professor.idNumber())
        .arg(professor.fullName());
}