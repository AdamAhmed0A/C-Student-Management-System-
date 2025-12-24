#ifndef CSV_STUDENT_DAO_H
#define CSV_STUDENT_DAO_H

#include "dao/student_dao.h"
#include "../../UniManage/models/student_data.h"
#include <QList>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

class CsvStudentDAO : public StudentDAO
{
public:
    CsvStudentDAO(const QString& filePath);
    ~CsvStudentDAO() override = default;

    bool insert(StudentData& student) override;
    bool update(const StudentData& student) override;
    bool remove(int id) override;
    bool softDelete(int id) override;
    bool restore(int id) override;
    QList<StudentData> getAll() override;
    QList<StudentData> getDeleted() override;
    StudentData getById(int id) override;
    StudentData getByUserId(int userId) override;
    StudentData getByIdNumber(const QString& idNumber) override;

private:
    QString m_filePath;
    QList<StudentData> m_students;
    int m_nextId;

    void loadData();
    bool saveData();
    bool ensureFileExists();
    QStringList getExpectedHeaders() const;
    StudentData parseLine(const QString& line) const;
    QString studentToCsvLine(const StudentData& student) const;
    QDateTime parseDateTime(const QString& str) const;
    QString dateTimeToString(const QDateTime& dt) const;
};

#endif // CSV_STUDENT_DAO_H