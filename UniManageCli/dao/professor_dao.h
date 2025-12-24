#ifndef PROFESSOR_DAO_H
#define PROFESSOR_DAO_H

#include "../UniManage/models/professor.h"
#include <QList>
#include <QString>

class ProfessorDAO
{
public:
    virtual ~ProfessorDAO() = default;

    virtual bool insert(Professor& professor) = 0;
    virtual bool update(const Professor& professor) = 0;
    virtual bool remove(int id) = 0;
    virtual QList<Professor> getAll() = 0;
    virtual Professor getById(int id) = 0;
    virtual Professor getByUserId(int userId) = 0;
    virtual Professor getByIdNumber(const QString& idNumber) = 0;
};

#endif // PROFESSOR_DAO_H