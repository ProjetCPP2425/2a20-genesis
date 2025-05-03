#ifndef QRCODE_H
#define QRCODE_H

#include <QString>

class QrCodeGenerator
{
public:
    static bool genererQRCode(const QString &texte, const QString &cheminFichier);
};

#endif // QRCODE_H
