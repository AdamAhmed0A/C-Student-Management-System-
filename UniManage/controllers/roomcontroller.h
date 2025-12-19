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
};

#endif // ROOMCONTROLLER_H
