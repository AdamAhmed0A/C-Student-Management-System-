#include "section.h"

Section::Section()
    : m_id(0), m_courseId(0), m_capacity(40), m_semesterId(0)
{
}

Section::Section(int id, int courseId, int capacity, int semesterId)
{
    m_id = id;
    m_courseId = courseId;
    m_capacity = capacity;
    m_semesterId = semesterId;
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
