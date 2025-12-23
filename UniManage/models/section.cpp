#include "section.h"

Section::Section()
    : m_id(0), m_name(""), m_courseId(0), m_capacity(40), m_semesterId(0), m_academicLevelId(0)
{
}

Section::Section(int id, const QString& name, int courseId, int capacity, int semesterId)
{
    m_id = id;
    m_name = name;
    m_courseId = courseId;
    m_capacity = capacity;
    m_semesterId = semesterId;
    m_academicLevelId = 0; // Initialize explicitly
}

QString Section::name() const {
    return m_name;
}

void Section::setName(const QString& name) {
    m_name = name;
}

int Section::id() const {
    return m_id;
}

void Section::setId(int id) {
    m_id = id;
}

int Section::courseId() const {
    return m_courseId;
}

void Section::setCourseId(int courseId) {
    m_courseId = courseId;
}

int Section::capacity() const {
    return m_capacity;
}

void Section::setCapacity(int capacity) {
    m_capacity = capacity;
}

int Section::semesterId() const {
    return m_semesterId;
}

void Section::setSemesterId(int semesterId) {
    m_semesterId = semesterId;
}

QDateTime Section::createdAt() const {
    return m_createdAt;
}

void Section::setCreatedAt(const QDateTime& createdAt) {
    m_createdAt = createdAt;
}

QDateTime Section::updatedAt() const {
    return m_updatedAt;
}

void Section::setUpdatedAt(const QDateTime& updatedAt) {
    m_updatedAt = updatedAt;
}

QString Section::courseName() const {
    return m_courseName;
}

void Section::setCourseName(const QString& courseName) {
    m_courseName = courseName;
}

int Section::academicLevelId() const {
    return m_academicLevelId;
}

void Section::setAcademicLevelId(int levelId) {
    m_academicLevelId = levelId;
}
