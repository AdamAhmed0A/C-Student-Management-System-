#ifndef PAYMENTCONTROLLER_H
#define PAYMENTCONTROLLER_H

#include "../models/payment.h"
#include <QList>

class PaymentController
{
public:
    PaymentController();

    bool addPayment(const Payment& payment);
    bool updatePayment(const Payment& payment);
    bool deletePayment(int id);

    QList<Payment> getAllPayments();
    QList<Payment> getPaymentsByStudent(int studentId);
};

#endif // PAYMENTCONTROLLER_H
