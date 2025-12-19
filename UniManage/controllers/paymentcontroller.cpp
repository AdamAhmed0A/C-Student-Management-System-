#include "paymentcontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

PaymentController::PaymentController() {}

bool PaymentController::addPayment(const Payment& payment)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::INSERT_PAYMENT);
    query.addBindValue(payment.studentId());
    query.addBindValue(payment.amount());
    query.addBindValue(payment.date().isValid() ? payment.date().toString(Qt::ISODate) : QString());
    query.addBindValue(payment.year().isValid() ? payment.year().toString(Qt::ISODate) : QString());
    query.addBindValue(payment.method());
    query.addBindValue(payment.status());
    query.addBindValue(payment.notes());

    if (!query.exec()) {
        qDebug() << "addPayment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool PaymentController::updatePayment(const Payment& payment)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::UPDATE_PAYMENT);
    query.addBindValue(payment.amount());
    query.addBindValue(payment.method());
    query.addBindValue(payment.status());
    query.addBindValue(payment.notes());
    query.addBindValue(payment.id());

    if (!query.exec()) {
        qDebug() << "updatePayment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool PaymentController::deletePayment(int id)
{
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::DELETE_PAYMENT);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "deletePayment failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<Payment> PaymentController::getAllPayments()
{
    QList<Payment> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);

    if (!query.exec(Queries::SELECT_ALL_PAYMENTS)) {
        qDebug() << "getAllPayments failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        Payment p;
        p.setId(query.value("id").toInt());
        p.setStudentId(query.value("student_id").toInt());
        p.setAmount(query.value("amount").toFloat());

        QDateTime dt = query.value("date").toDateTime();
        if (dt.isValid()) p.setDate(dt);

        QDateTime yr = query.value("year").toDateTime();
        if (yr.isValid()) p.setYear(yr);

        p.setMethod(query.value("method").toString());
        p.setStatus(query.value("status").toString());
        p.setNotes(query.value("notes").toString());

        QDateTime created = query.value("created_at").toDateTime();
        if (created.isValid()) p.setCreatedAt(created);

        p.setStudentNumber(query.value("student_number").toString());

        list.append(p);
    }
    return list;
}

QList<Payment> PaymentController::getPaymentsByStudent(int studentId)
{
    QList<Payment> list;
    QSqlDatabase& db = DBConnection::instance().database();
    QSqlQuery query(db);
    query.prepare(Queries::SELECT_PAYMENTS_BY_STUDENT);
    query.addBindValue(studentId);

    if (!query.exec()) {
        qDebug() << "getPaymentsByStudent failed:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        Payment p;
        p.setId(query.value("id").toInt());
        p.setStudentId(query.value("student_id").toInt());
        p.setAmount(query.value("amount").toFloat());

        QDateTime dt = query.value("date").toDateTime();
        if (dt.isValid()) p.setDate(dt);

        QDateTime yr = query.value("year").toDateTime();
        if (yr.isValid()) p.setYear(yr);

        p.setMethod(query.value("method").toString());
        p.setStatus(query.value("status").toString());
        p.setNotes(query.value("notes").toString());

        QDateTime created = query.value("created_at").toDateTime();
        if (created.isValid()) p.setCreatedAt(created);

        list.append(p);
    }
    return list;
}
