#include "schedule.h"

/**
 * Default Constructor
 * Initializes a new schedule slot with default ID values
 */
Schedule::Schedule() : m_id(0), m_courseId(0), m_roomId(0), m_professorId(0) {}

/**
 * @return The unique ID of the schedule slot
 */
int Schedule::id() const { return m_id; }

/**
 * Sets the unique ID of the schedule slot
 * @param id - The new ID
 */
void Schedule::setId(int id) { m_id = id; }

/**
 * @return The ID of the course scheduled
 */
int Schedule::courseId() const { return m_courseId; }

/**
 * Sets the ID of the course
 * @param courseId - The new course ID
 */
void Schedule::setCourseId(int courseId) { m_courseId = courseId; }

/**
 * @return The ID of the room where the class takes place
 */
int Schedule::roomId() const { return m_roomId; }

/**
 * Sets the room ID
 * @param roomId - The new room ID
 */
void Schedule::setRoomId(int roomId) { m_roomId = roomId; }

/**
 * @return The ID of the professor teaching the class
 */
int Schedule::professorId() const { return m_professorId; }

/**
 * Sets the professor's ID
 * @param professorId - The new professor ID
 */
void Schedule::setProfessorId(int professorId) { m_professorId = professorId; }

/**
 * @return The day of the week (e.g., "Monday")
 */
QString Schedule::dayOfWeek() const { return m_dayOfWeek; }

/**
 * Sets the day of the week
 * @param day - The day string
 */
void Schedule::setDayOfWeek(const QString& day) { m_dayOfWeek = day; }

/**
 * @return The start time of the class
 */
QTime Schedule::startTime() const { return m_startTime; }

/**
 * Sets the start time
 * @param time - The start time
 */
void Schedule::setStartTime(const QTime& time) { m_startTime = time; }

/**
 * @return The end time of the class
 */
QTime Schedule::endTime() const { return m_endTime; }

/**
 * Sets the end time
 * @param time - The end time
 */
void Schedule::setEndTime(const QTime& time) { m_endTime = time; }

/**
 * @return The name of the course (derived/joined)
 */
QString Schedule::courseName() const { return m_courseName; }

/**
 * Sets the course name
 * @param name - The course name
 */
void Schedule::setCourseName(const QString& name) { m_courseName = name; }

/**
 * @return The name of the room (derived/joined)
 */
QString Schedule::roomName() const { return m_roomName; }

/**
 * Sets the room name
 * @param name - The room name
 */
void Schedule::setRoomName(const QString& name) { m_roomName = name; }

/**
 * @return The name of the professor (derived/joined)
 */
QString Schedule::professorName() const { return m_professorName; }

/**
 * Sets the professor name
 * @param name - The professor name
 */
void Schedule::setProfessorName(const QString& name) { m_professorName = name; }
