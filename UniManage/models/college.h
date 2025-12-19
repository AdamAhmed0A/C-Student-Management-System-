#ifndef COLLEGE_H
#define COLLEGE_H

#include <QString>

class College
{
public:
    College();
    College(int id, const QString& name, const QString& code);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QString code() const;
    void setCode(const QString& code);

private:
    int m_id;
    QString m_name;
    QString m_code;
};

#endif // COLLEGE_H
