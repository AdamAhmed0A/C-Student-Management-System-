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

private:
    int m_id;
    QString m_name;
    QString m_type;
    int m_capacity;
};

#endif // ROOM_H
