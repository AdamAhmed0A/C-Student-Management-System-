#include "roomcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RoomController::RoomController() {}

bool RoomController::addRoom(const Room& room)
{
    QSqlQuery query(DBConnection::instance().database());
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
    return query.exec();
}

bool RoomController::updateRoom(const Room& room)
{
    QSqlQuery query(DBConnection::instance().database());
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
    return query.exec();
}

bool RoomController::deleteRoom(int id)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_ROOM);
    query.addBindValue(id);
    return query.exec();
}

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

bool RoomController::deleteRoomSpecs(int roomId)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_ROOM_SPECS);
    query.addBindValue(roomId);
    return query.exec();
}
