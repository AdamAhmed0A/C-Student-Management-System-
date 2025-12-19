#include "room.h"

Room::Room() : m_id(0), m_capacity(0) {}

Room::Room(int id, const QString& name, const QString& type, int capacity)
    : m_id(id), m_name(name), m_type(type), m_capacity(capacity) {}

int Room::id() const { return m_id; }
void Room::setId(int id) { m_id = id; }

QString Room::name() const { return m_name; }
void Room::setName(const QString& name) { m_name = name; }

QString Room::type() const { return m_type; }
void Room::setType(const QString& type) { m_type = type; }

int Room::capacity() const { return m_capacity; }
void Room::setCapacity(int capacity) { m_capacity = capacity; }
