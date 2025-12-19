#ifndef PROFESSORCONTROLLER_H
#define PROFESSORCONTROLLER_H

#include "../models/professor.h"
#include <QList>

class ProfessorController
{
public:
    ProfessorController();
    bool addProfessor(const Professor& prof);
    bool updateProfessor(const Professor& prof);
    bool deleteProfessor(int id);
    QList<Professor> getAllProfessors();
};

#endif // PROFESSORCONTROLLER_H
