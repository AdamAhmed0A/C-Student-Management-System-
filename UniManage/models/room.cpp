#include "room.h"

/**
 * Default Constructor
 * Initializes a new room instance with default values (id=0, capacity=0, etc.)
 */
Room::Room() : m_id(0), m_capacity(0), m_acUnits(0), m_fansCount(0), m_lightingPoints(0), m_computersCount(0) {}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the room
 * @param name - The name of the room (e.g., "Hall A")
 * @param type - The type of the room (e.g., "Lecture Hall", "Lab")
 * @param capacity - The maximum student capacity
 */
Room::Room(int id, const QString& name, const QString& type, int capacity)
    : m_id(id), m_name(name), m_type(type), m_capacity(capacity), m_acUnits(0), m_fansCount(0), m_lightingPoints(0), m_computersCount(0) {}

/**
 * @return The unique ID of the room
 */
int Room::id() const { return m_id; }

/**
 * Sets the unique ID of the room
 * @param id - The new ID
 */
void Room::setId(int id) { m_id = id; }

/**
 * @return The name of the room
 */
QString Room::name() const { return m_name; }

/**
 * Sets the name of the room
 * @param name - The new name
 */
void Room::setName(const QString& name) { m_name = name; }

/**
 * @return The type of the room
 */
QString Room::type() const { return m_type; }

/**
 * Sets the type of the room
 * @param type - The new type
 */
void Room::setType(const QString& type) { m_type = type; }

/**
 * @return The student capacity of the room
 */
int Room::capacity() const { return m_capacity; }

/**
 * Sets the student capacity
 * @param capacity - The new capacity
 */
void Room::setCapacity(int capacity) { m_capacity = capacity; }

/**
 * @return The number of AC units in the room
 */
int Room::acUnits() const { return m_acUnits; }

/**
 * Sets the number of AC units
 * @param count - The count
 */
void Room::setAcUnits(int count) { m_acUnits = count; }

/**
 * @return The number of fans in the room
 */
int Room::fansCount() const { return m_fansCount; }

/**
 * Sets the number of fans
 * @param count - The count
 */
void Room::setFansCount(int count) { m_fansCount = count; }

/**
 * @return The number of lighting points in the room
 */
int Room::lightingPoints() const { return m_lightingPoints; }

/**
 * Sets the number of lighting points
 * @param count - The count
 */
void Room::setLightingPoints(int count) { m_lightingPoints = count; }

/**
 * @return The number of computers in the room
 */
int Room::computersCount() const { return m_computersCount; }

/**
 * Sets the number of computers
 * @param count - The count
 */
void Room::setComputersCount(int count) { m_computersCount = count; }

/**
 * @return A description of the seating arrangement
 */
QString Room::seatingDescription() const { return m_seatingDescription; }

/**
 * Sets the seating description
 * @param desc - The description
 */
void Room::setSeatingDescription(const QString& desc) { m_seatingDescription = desc; }

/**
 * @return The unique code for the room
 */
QString Room::code() const { return m_code; }

/**
 * Sets the unique room code
 * @param code - The new code
 */
void Room::setCode(const QString& code) { m_code = code; }
