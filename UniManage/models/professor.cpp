#include "professor.h"

Professor::Professor() : m_id(0), m_userId(0) {}

Professor::Professor(int id, int userId, const QString& specialization, const QString& title)
    : m_id(id), m_userId(userId), m_specialization(specialization), m_title(title) {}

int Professor::id() const { return m_id; }
void Professor::setId(int id) { m_id = id; }

int Professor::userId() const { return m_userId; }
void Professor::setUserId(int userId) { m_userId = userId; }

QString Professor::specialization() const { return m_specialization; }
void Professor::setSpecialization(const QString& specialization) { m_specialization = specialization; }

QString Professor::title() const { return m_title; }
void Professor::setTitle(const QString& title) { m_title = title; }

QString Professor::personalInfo() const { return m_personalInfo; }
void Professor::setPersonalInfo(const QString& info) { m_personalInfo = info; }

QString Professor::fullName() const { return m_fullName; }
void Professor::setFullName(const QString& name) { m_fullName = name; }
