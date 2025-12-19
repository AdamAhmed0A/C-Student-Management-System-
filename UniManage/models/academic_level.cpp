#include "academic_level.h"

AcademicLevel::AcademicLevel() : m_id(0), m_levelNumber(1) {}

AcademicLevel::AcademicLevel(int id, const QString& name, int levelNumber)
    : m_id(id), m_name(name), m_levelNumber(levelNumber) {}

int AcademicLevel::id() const { return m_id; }
void AcademicLevel::setId(int id) { m_id = id; }

QString AcademicLevel::name() const { return m_name; }
void AcademicLevel::setName(const QString& name) { m_name = name; }

int AcademicLevel::levelNumber() const { return m_levelNumber; }
void AcademicLevel::setLevelNumber(int levelNumber) { m_levelNumber = levelNumber; }
