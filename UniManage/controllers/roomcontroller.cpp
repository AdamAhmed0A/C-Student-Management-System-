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
    return query.exec();
}

bool RoomController::updateRoom(const Room& room)
{
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_ROOM);
    query.addBindValue(room.name());
    query.addBindValue(room.type());
    query.addBindValue(room.capacity());
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
            list.append(Room(query.value("id").toInt(),
                            query.value("name").toString(),
                            query.value("type").toString(),
                            query.value("capacity").toInt()));
        }
    }
    return list;
}
