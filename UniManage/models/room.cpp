#include "room.h"

Room::Room() : m_id(0), m_capacity(0), m_acUnits(0), m_fansCount(0), m_lightingPoints(0), m_computersCount(0) {}

Room::Room(int id, const QString& name, const QString& type, int capacity)
    : m_id(id), m_name(name), m_type(type), m_capacity(capacity), m_acUnits(0), m_fansCount(0), m_lightingPoints(0), m_computersCount(0) {}

int Room::id() const { return m_id; }
void Room::setId(int id) { m_id = id; }

QString Room::name() const { return m_name; }
void Room::setName(const QString& name) { m_name = name; }

QString Room::type() const { return m_type; }
void Room::setType(const QString& type) { m_type = type; }

int Room::capacity() const { return m_capacity; }
void Room::setCapacity(int capacity) { m_capacity = capacity; }

int Room::acUnits() const { return m_acUnits; }
void Room::setAcUnits(int count) { m_acUnits = count; }

int Room::fansCount() const { return m_fansCount; }
void Room::setFansCount(int count) { m_fansCount = count; }

int Room::lightingPoints() const { return m_lightingPoints; }
void Room::setLightingPoints(int count) { m_lightingPoints = count; }

int Room::computersCount() const { return m_computersCount; }
void Room::setComputersCount(int count) { m_computersCount = count; }

QString Room::seatingDescription() const { return m_seatingDescription; }
void Room::setSeatingDescription(const QString& desc) { m_seatingDescription = desc; }

QString Room::code() const { return m_code; }
void Room::setCode(const QString& code) { m_code = code; }
