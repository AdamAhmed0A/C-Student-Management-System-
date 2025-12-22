#include "college.h"

College::College() : m_id(0) {}

College::College(int id, const QString& name, const QString& code, double tuitionFees)
    : m_id(id), m_name(name), m_code(code), m_tuitionFees(tuitionFees) {}

int College::id() const { return m_id; }
void College::setId(int id) { m_id = id; }

QString College::name() const { return m_name; }
void College::setName(const QString& name) { m_name = name; }

QString College::code() const { return m_code; }
void College::setCode(const QString& code) { m_code = code; }

double College::tuitionFees() const { return m_tuitionFees; }
void College::setTuitionFees(double fees) { m_tuitionFees = fees; }
