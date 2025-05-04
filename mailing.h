#ifndef MAILING_H
#define MAILING_H


#include <QString>

class Mailing {
public:
    static bool sendMail(const QString &receiverEmail, const QString &subject, const QString &body);
};


#endif // MAILING_H
