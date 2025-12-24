#include <QStringConverter>
#include "csv_user_dao.h"
#include <QTextStream>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>
#include <algorithm>

CsvUserDAO::CsvUserDAO(const QString& filePath)
    : m_filePath(filePath), m_nextId(1)
{
    ensureFileExists();
    loadData();
}

bool CsvUserDAO::insert(User& user)
{
    user.setId(m_nextId++);
    user.setCreatedAt(QDateTime::currentDateTime());
    user.setUpdatedAt(QDateTime::currentDateTime());
    user.setPassword(hashPassword(user.password()));
    m_users.append(user);
    return saveData();
}

bool CsvUserDAO::update(const User& user)
{
    for (auto& u : m_users) {
        if (u.id() == user.id()) {
            u = user;
            u.setUpdatedAt(QDateTime::currentDateTime());
            return saveData();
        }
    }
    return false;
}

bool CsvUserDAO::remove(int id)
{
    auto it = std::remove_if(m_users.begin(), m_users.end(),
                             [id](const User& u) { return u.id() == id; });
    if (it != m_users.end()) {
        m_users.erase(it, m_users.end());
        return saveData();
    }
    return false;
}

QList<User> CsvUserDAO::getAll()
{
    return m_users;
}

User CsvUserDAO::getById(int id)
{
    for (const auto& u : m_users) {
        if (u.id() == id) {
            return u;
        }
    }
    return User();
}

User CsvUserDAO::getByUsername(const QString& username)
{
    for (const auto& u : m_users) {
        if (u.username() == username) {
            return u;
        }
    }
    return User();
}

User CsvUserDAO::authenticate(const QString& username, const QString& password)
{
    QString hashedPassword = hashPassword(password);
    for (const auto& u : m_users) {
        if (u.username() == username && u.password() == hashedPassword) {
            return u;
        }
    }
    return User();
}

void CsvUserDAO::loadData()
{
    m_users.clear();
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << m_filePath;
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // Read header
    if (!in.atEnd()) {
        QString header = in.readLine();
        QStringList headers = header.split(',');
        QStringList expected = getExpectedHeaders();
        if (headers != expected) {
            qWarning() << "CSV headers mismatch. Expected:" << expected << "Got:" << headers;
        }
    }

    // Read data
    while (!in.atEnd()) {
        QString line = in.readLine();
        line = line.trimmed();
        if (!line.isEmpty()) {
            User user = parseLine(line);
            if (user.id() > 0) {
                m_users.append(user);
                m_nextId = std::max(m_nextId, user.id() + 1);
            }
        }
    }
    file.close();
}

bool CsvUserDAO::saveData()
{
    QString tempPath = m_filePath + ".tmp";
    QFile file(tempPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open temp file:" << tempPath;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Write header
    out << getExpectedHeaders().join(',') << '\n';

    // Write data
    for (const auto& user : m_users) {
        out << userToCsvLine(user) << '\n';
    }

    file.close();

    // Atomic rename
    if (QFile::exists(m_filePath)) {
        QFile::remove(m_filePath);
    }
    return QFile::rename(tempPath, m_filePath);
}

bool CsvUserDAO::ensureFileExists()
{
    if (QFile::exists(m_filePath)) {
        return true;
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << getExpectedHeaders().join(',') << '\n';
    file.close();
    return true;
}

QStringList CsvUserDAO::getExpectedHeaders() const
{
    return {"id", "full_name", "username", "password", "role", "created_at", "updated_at"};
}

User CsvUserDAO::parseLine(const QString& line) const
{
    QStringList parts = line.split(',');
    if (parts.size() < 7) return User();

    User user;
    user.setId(parts.at(0).toInt());
    user.setFullName(parts.at(1));
    user.setUsername(parts.at(2));
    user.setPassword(parts.at(3));
    user.setRole(parts.at(4));
    user.setCreatedAt(parseDateTime(parts.at(5)));
    user.setUpdatedAt(parseDateTime(parts.at(6)));
    return user;
}

QString CsvUserDAO::userToCsvLine(const User& user) const
{
    return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(user.id())
        .arg(user.fullName())
        .arg(user.username())
        .arg(user.password())
        .arg(user.role())
        .arg(dateTimeToString(user.createdAt()))
        .arg(dateTimeToString(user.updatedAt()));
}

QDateTime CsvUserDAO::parseDateTime(const QString& str) const
{
    QDate date = QDate::fromString(str, Qt::ISODate);
    if (date.isValid()) {
        return QDateTime(date, QTime());
    }
    return QDateTime::fromString(str, Qt::ISODate);
}

QString CsvUserDAO::dateTimeToString(const QDateTime& dt) const
{
    return dt.date().toString(Qt::ISODate);
}

QString CsvUserDAO::hashPassword(const QString& password) const
{
    QByteArray data = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return QString::fromLatin1(hash.toHex());
}