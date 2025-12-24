#include "news.h"

/**
 * Default Constructor
 * Initializes a new news item with default ID 0
 */
News::News()
    : m_id(0)
{
}

/**
 * Parameterized Constructor
 * @param id - The unique ID of the news item
 * @param title - The title/headline of the news
 * @param body - The main content/body of the news
 */
News::News(int id, const QString& title, const QString& body)
{
    m_id = id;
    m_title = title;
    m_body = body;
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime();
}

/**
 * @return The unique ID of the news item
 */
int News::id() const
{
    return m_id;
}

/**
 * Sets the unique ID of the news item
 * @param id - The new ID
 */
void News::setId(int id)
{
    m_id = id;
}

/**
 * @return The title of the news item
 */
QString News::title() const
{
    return m_title;
}

/**
 * Sets the title of the news item
 * @param title - The new title
 */
void News::setTitle(const QString& title)
{
    m_title = title;
}

/**
 * @return The body content of the news item
 */
QString News::body() const
{
    return m_body;
}

/**
 * Sets the body content of the news item
 * @param body - The new body content
 */
void News::setBody(const QString& body)
{
    m_body = body;
}

/**
 * @return The timestamp when the news was created
 */
QDateTime News::createdAt() const
{
    return m_createdAt;
}

/**
 * Sets the creation timestamp
 * @param createdAt - The new timestamp
 */
void News::setCreatedAt(const QDateTime& createdAt)
{
    m_createdAt = createdAt;
}

/**
 * @return The timestamp when the news was last updated
 */
QDateTime News::updatedAt() const
{
    return m_updatedAt;
}

/**
 * Sets the update timestamp
 * @param updatedAt - The new timestamp
 */
void News::setUpdatedAt(const QDateTime& updatedAt)
{
    m_updatedAt = updatedAt;
}
