#include "department.h"

Department::Department() : m_id(0), m_collegeId(0) {}

Department::Department(int id, const QString& name, int collegeId, const QString& code)
    : m_id(id), m_name(name), m_collegeId(collegeId), m_code(code) {}

int Department::id() const { return m_id; }
void Department::setId(int id) { m_id = id; }

QString Department::name() const { return m_name; }
void Department::setName(const QString& name) { m_name = name; }

int Department::collegeId() const { return m_collegeId; }
void Department::setCollegeId(int collegeId) { m_collegeId = collegeId; }

QString Department::code() const { return m_code; }
void Department::setCode(const QString& code) { m_code = code; }

QString Department::collegeName() const { return m_collegeName; }
void Department::setCollegeName(const QString& name) { m_collegeName = name; }
