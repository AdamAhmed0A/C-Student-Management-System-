#ifndef NEWS_H
#define NEWS_H

#include <QString>
#include <QDateTime>

class News
{
public:
    News();
    News(int id, const QString& title, const QString& body);

    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString& title);

    QString body() const;
    void setBody(const QString& body);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime& updatedAt);

private:
    int m_id;
    QString m_title;
    QString m_body;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
};

#endif // NEWS_H
