#include "academic_level.h"

/**
 * Default Constructor
 * Initializes a new instance with default values (id=0, levelNumber=1)
 */
AcademicLevel::AcademicLevel() : m_id(0), m_levelNumber(1) {}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the academic level
 * @param name - The name of the academic level (e.g., "Year 1")
 * @param levelNumber - The numeric representation of the level
 */
AcademicLevel::AcademicLevel(int id, const QString& name, int levelNumber)
    : m_id(id), m_name(name), m_levelNumber(levelNumber) {}

/**
 * @return The unique ID of the academic level
 */
int AcademicLevel::id() const { return m_id; }

/**
 * Sets the unique ID of the academic level
 * @param id - The new ID
 */
void AcademicLevel::setId(int id) { m_id = id; }

/**
 * @return The name of the academic level
 */
QString AcademicLevel::name() const { return m_name; }

/**
 * Sets the name of the academic level
 * @param name - The new name
 */
void AcademicLevel::setName(const QString& name) { m_name = name; }

/**
 * @return The numeric value of the academic level
 */
int AcademicLevel::levelNumber() const { return m_levelNumber; }

/**
 * Sets the numeric value of the academic level
 * @param levelNumber - The new numeric level
 */
void AcademicLevel::setLevelNumber(int levelNumber) { m_levelNumber = levelNumber; }
