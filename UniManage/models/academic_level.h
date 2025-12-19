#ifndef ACADEMICLEVEL_H
#define ACADEMICLEVEL_H

#include <QString>

class AcademicLevel
{
public:
    AcademicLevel();
    AcademicLevel(int id, const QString& name, int levelNumber);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    int levelNumber() const;
    void setLevelNumber(int levelNumber);

private:
    int m_id;
    QString m_name;
    int m_levelNumber;
};

#endif // ACADEMICLEVEL_H
