#ifndef CSV_PROFESSOR_DAO_H
#define CSV_PROFESSOR_DAO_H

#include "dao/professor_dao.h"
#include "../../UniManage/models/professor.h"
#include <QList>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

class CsvProfessorDAO : public ProfessorDAO
{
public:
    CsvProfessorDAO(const QString& filePath);
    ~CsvProfessorDAO() override = default;

    bool insert(Professor& professor) override;
    bool update(const Professor& professor) override;
    bool remove(int id) override;
    QList<Professor> getAll() override;
    Professor getById(int id) override;
    Professor getByUserId(int userId) override;
    Professor getByIdNumber(const QString& idNumber) override;

private:
    QString m_filePath;
    QList<Professor> m_professors;
    int m_nextId;

    void loadData();
    bool saveData();
    bool ensureFileExists();
    QStringList getExpectedHeaders() const;
    Professor parseLine(const QString& line) const;
    QString professorToCsvLine(const Professor& professor) const;
};

#endif // CSV_PROFESSOR_DAO_H