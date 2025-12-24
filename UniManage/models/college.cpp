#include "college.h"

/**
 * Default Constructor
 * Initializes a new instance with default ID 0
 */
College::College() : m_id(0) {}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the college/faculty
 * @param name - The name of the college (e.g., "Faculty of Engineering")
 * @param code - A short code for the college (e.g., "ENG")
 * @param tuitionFees - The base tuition fees for this college
 */
College::College(int id, const QString& name, const QString& code, double tuitionFees)
    : m_id(id), m_name(name), m_code(code), m_tuitionFees(tuitionFees) {}

/**
 * @return The unique ID of the college
 */
int College::id() const { return m_id; }

/**
 * Sets the unique ID of the college
 * @param id - The new ID
 */
void College::setId(int id) { m_id = id; }

/**
 * @return The name of the college
 */
QString College::name() const { return m_name; }

/**
 * Sets the name of the college
 * @param name - The new name
 */
void College::setName(const QString& name) { m_name = name; }

/**
 * @return The short code/abbreviation of the college
 */
QString College::code() const { return m_code; }

/**
 * Sets the short code/abbreviation of the college
 * @param code - The new code
 */
void College::setCode(const QString& code) { m_code = code; }

/**
 * @return The base tuition fees for the college
 */
double College::tuitionFees() const { return m_tuitionFees; }

/**
 * Sets the base tuition fees for the college
 * @param fees - The new fees
 */
void College::setTuitionFees(double fees) { m_tuitionFees = fees; }
