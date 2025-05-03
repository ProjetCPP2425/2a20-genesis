#include "qrcode.h"
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <qrcodegen.hpp> // Lib externe Nayuki, à inclure au projet

using namespace qrcodegen;

bool QrCodeGenerator::genererQRCode(const QString &texte, const QString &cheminFichier)
{
    QrCode qr = QrCode::encodeText(texte.toUtf8().constData(), QrCode::Ecc::LOW);
    const int scale = 10;
    int size = qr.getSize() * scale;

    QImage image(size, size, QImage::Format_RGB888);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    for (int y = 0; y < qr.getSize(); ++y) {
        for (int x = 0; x < qr.getSize(); ++x) {
            if (qr.getModule(x, y)) {
                painter.drawRect(x * scale, y * scale, scale, scale);
            }
        }
    }

    return image.save(cheminFichier);
}
