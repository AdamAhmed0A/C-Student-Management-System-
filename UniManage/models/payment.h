#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>
#include <QDateTime>

class Payment
{
public:
    Payment();
    Payment(int id, int studentId, float amount, const QDateTime &date, const QDateTime &year,
            const QString &method, const QString &status, const QString &notes,
            const QDateTime &createdAt, const QString &studentNumber);

    int id() const;
    void setId(int id);

    int studentId() const;
    void setStudentId(int studentId);

    float amount() const;
    void setAmount(float amount);

    QDateTime date() const;
    void setDate(const QDateTime& date);

    QDateTime year() const;
    void setYear(const QDateTime& year);

    QString method() const;
    void setMethod(const QString& method);

    QString status() const;
    void setStatus(const QString& status);

    QString notes() const;
    void setNotes(const QString& notes);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime& createdAt);

    // Helper
    QString studentNumber() const;
    void setStudentNumber(const QString& studentNumber);

private:
    int m_id;
    int m_studentId;
    float m_amount;
    QDateTime m_date;
    QDateTime m_year;
    QString m_method;
    QString m_status;
    QString m_notes;
    QDateTime m_createdAt;
    QString m_studentNumber;
};

#endif // PAYMENT_H
