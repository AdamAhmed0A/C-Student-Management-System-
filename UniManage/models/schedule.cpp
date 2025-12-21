#include "schedule.h"

Schedule::Schedule() : m_id(0), m_courseId(0), m_roomId(0), m_professorId(0) {}

int Schedule::id() const { return m_id; }
void Schedule::setId(int id) { m_id = id; }

int Schedule::courseId() const { return m_courseId; }
void Schedule::setCourseId(int courseId) { m_courseId = courseId; }

int Schedule::roomId() const { return m_roomId; }
void Schedule::setRoomId(int roomId) { m_roomId = roomId; }

int Schedule::professorId() const { return m_professorId; }
void Schedule::setProfessorId(int professorId) { m_professorId = professorId; }

QString Schedule::dayOfWeek() const { return m_dayOfWeek; }
void Schedule::setDayOfWeek(const QString& day) { m_dayOfWeek = day; }

QTime Schedule::startTime() const { return m_startTime; }
void Schedule::setStartTime(const QTime& time) { m_startTime = time; }

QTime Schedule::endTime() const { return m_endTime; }
void Schedule::setEndTime(const QTime& time) { m_endTime = time; }

QString Schedule::courseName() const { return m_courseName; }
void Schedule::setCourseName(const QString& name) { m_courseName = name; }

QString Schedule::roomName() const { return m_roomName; }
void Schedule::setRoomName(const QString& name) { m_roomName = name; }

QString Schedule::professorName() const { return m_professorName; }
void Schedule::setProfessorName(const QString& name) { m_professorName = name; }
