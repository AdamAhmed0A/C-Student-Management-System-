#include "roomcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../database/persistence.h"

/**
 * Constructor for the RoomController class
 */
RoomController::RoomController() {}

/**
 * Adds a new room/facility to the database
 * Logs the creation event after successful insertion
 * @param room - The Room object containing details
 * @return True if successful, otherwise false
 */
bool RoomController::addRoom(const Room& room)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::INSERT_ROOM);
    query.addBindValue(room.name());
    query.addBindValue(room.type());
    query.addBindValue(room.capacity());
    query.addBindValue(room.acUnits());
    query.addBindValue(room.fansCount());
    query.addBindValue(room.lightingPoints());
    query.addBindValue(room.computersCount());
    query.addBindValue(room.seatingDescription());
    query.addBindValue(room.code());
    if (query.exec()) {
        Persistence::logChange(room.type(), "Create", query.lastInsertId().toInt(), room.name());
        return true;
    }
    return false;
}

/**
 * Updates an existing room/facility in the database
 * Logs the update event after successful execution
 * @param room - The Room object with updated details
 * @return True if successful, otherwise false
 */
bool RoomController::updateRoom(const Room& room)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::UPDATE_ROOM);
    query.addBindValue(room.name());
    query.addBindValue(room.type());
    query.addBindValue(room.capacity());
    query.addBindValue(room.acUnits());
    query.addBindValue(room.fansCount());
    query.addBindValue(room.lightingPoints());
    query.addBindValue(room.computersCount());
    query.addBindValue(room.seatingDescription());
    query.addBindValue(room.code());
    query.addBindValue(room.id());
    if (query.exec()) {
        Persistence::logChange(room.type(), "Edit", room.id(), room.name());
        return true;
    }
    return false;
}

/**
 * Deletes a room/facility from the database
 * Logs the deletion event after successful execution
 * @param id - The ID of the room to delete
 * @return True if successful, otherwise false
 */
bool RoomController::deleteRoom(int id)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::DELETE_ROOM);
    query.addBindValue(id);
    if (query.exec()) {
        Persistence::logChange("Facility", "Delete", id, "Removed Hall/Lab");
        return true;
    }
    return false;
}

/**
 * Retrieves all rooms/facilities from the database
 * @return A list of Room objects
 */
QList<Room> RoomController::getAllRooms()
{
    QList<Room> list;
    QSqlQuery query(DBConnection::instance().database());
    if (query.exec(Queries::SELECT_ALL_ROOMS)) {
        while (query.next()) {
            Room r;
            r.setId(query.value("id").toInt());
            r.setName(query.value("name").toString());
            r.setType(query.value("type").toString());
            r.setCapacity(query.value("capacity").toInt());
            r.setAcUnits(query.value("ac_units").toInt());
            r.setFansCount(query.value("fans_count").toInt());
            r.setLightingPoints(query.value("lighting_points").toInt());
            r.setComputersCount(query.value("computers_count").toInt());
            r.setSeatingDescription(query.value("seating_description").toString());
            r.setCode(query.value("code").toString());
            list.append(r);
        }
    }
    return list;
}

/**
 * Adds a specification/equipment item to a room
 * @param roomId - The ID of the room
 * @param prodId - Product/Item unique code
 * @param name - Name of the equipment
 * @param desc - Description of the equipment
 * @return True if successful, otherwise false
 */
bool RoomController::addRoomSpec(int roomId, const QString& prodId, const QString& name, const QString& desc)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_ROOM_SPEC);
    query.addBindValue(roomId);
    query.addBindValue(prodId);
    query.addBindValue(name);
    query.addBindValue(desc);
    return query.exec();
}

/**
 * Retrieves all specifications/equipment for a specific room
 * @param roomId - The ID of the room
 * @return A list of RoomSpec objects
 */
QList<RoomSpec> RoomController::getRoomSpecs(int roomId)
{
    QList<RoomSpec> list;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_ROOM_SPECS);
    query.addBindValue(roomId);
    if (query.exec()) {
        while (query.next()) {
             RoomSpec s;
             s.id = query.value("id").toInt();
             s.roomId = query.value("room_id").toInt();
             s.productId = query.value("product_id").toString();
             s.productName = query.value("product_name").toString();
             s.description = query.value("product_description").toString();
             list.append(s);
        }
    }
    return list;
}

/**
 * Deletes all specifications/equipment for a specific room
 * @param roomId - The ID of the room
 * @return True if successful, otherwise false
 */
bool RoomController::deleteRoomSpecs(int roomId)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_ROOM_SPECS);
    query.addBindValue(roomId);
    return query.exec();
}

/**
 * Retrieves a room/facility by its ID
 * @param id - The ID of the room
 * @return The Room object if found, otherwise an empty object
 */
Room RoomController::getRoomById(int id)
{
    Room r;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_ROOM_BY_ID);
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        r.setId(query.value("id").toInt());
        r.setName(query.value("name").toString());
        r.setType(query.value("type").toString());
        r.setCapacity(query.value("capacity").toInt());
        r.setAcUnits(query.value("ac_units").toInt());
        r.setFansCount(query.value("fans_count").toInt());
        r.setLightingPoints(query.value("lighting_points").toInt());
        r.setComputersCount(query.value("computers_count").toInt());
        r.setSeatingDescription(query.value("seating_description").toString());
        r.setCode(query.value("code").toString());
    }
    return r;
}
