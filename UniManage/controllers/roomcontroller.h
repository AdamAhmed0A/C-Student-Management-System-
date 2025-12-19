#ifndef ROOMCONTROLLER_H
#define ROOMCONTROLLER_H

#include "../models/room.h"
#include <QList>

class RoomController
{
public:
    RoomController();
    bool addRoom(const Room& room);
    bool updateRoom(const Room& room);
    bool deleteRoom(int id);
    QList<Room> getAllRooms();
    
    // Spec management
    bool addRoomSpec(int roomId, const QString& prodId, const QString& name, const QString& desc);
    QList<RoomSpec> getRoomSpecs(int roomId);
    bool deleteRoomSpecs(int roomId);
};

#endif // ROOMCONTROLLER_H
