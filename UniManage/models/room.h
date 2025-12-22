#ifndef ROOM_H
#define ROOM_H

#include <QString>

class Room
{
public:
    Room();
    Room(int id, const QString& name, const QString& type, int capacity);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QString type() const;
    void setType(const QString& type);

    int capacity() const;
    void setCapacity(int capacity);

    int acUnits() const;
    void setAcUnits(int count);

    int fansCount() const;
    void setFansCount(int count);

    int lightingPoints() const;
    void setLightingPoints(int count);

    int computersCount() const;
    void setComputersCount(int count);

    QString seatingDescription() const;
    void setSeatingDescription(const QString& desc);

    QString code() const;
    void setCode(const QString& code);

private:
    int m_id;
    QString m_name;
    QString m_type;
    int m_capacity;
    int m_acUnits;
    int m_fansCount;
    int m_lightingPoints;
    int m_computersCount;
    QString m_seatingDescription;
    QString m_code;
};

class LectureHall : public Room {
public:
    LectureHall() { setType("Hall"); }
    // Specific to Hall: Seating Description
};

class Laboratory : public Room {
public:
    Laboratory() { setType("Lab"); }
    // Specific to Lab: Computers Count
};

struct RoomSpec {
    int id;
    int roomId;
    QString productId;
    QString productName;
    QString description;
};

#endif // ROOM_H
