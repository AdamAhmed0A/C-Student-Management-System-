#include "news.h"

News::News()
    : m_id(0)
{
}

News::News(int id, const QString& title, const QString& body)
{
    m_id = id;
    m_title = title;
    m_body = body;
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime();
}

int News::id() const
{
    return m_id;
}

void News::setId(int id)
{
    m_id = id;
}

QString News::title() const
{
    return m_title;
}

void News::setTitle(const QString& title)
{
    m_title = title;
}

QString News::body() const
{
    return m_body;
}

void News::setBody(const QString& body)
{
    m_body = body;
}

QDateTime News::createdAt() const
{
    return m_createdAt;
}

void News::setCreatedAt(const QDateTime& createdAt)
{
    m_createdAt = createdAt;
}

QDateTime News::updatedAt() const
{
    return m_updatedAt;
}

void News::setUpdatedAt(const QDateTime& updatedAt)
{
    m_updatedAt = updatedAt;
}
