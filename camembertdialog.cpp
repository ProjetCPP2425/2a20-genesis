#include "camembertdialog.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QtMath>

CamembertDialog::CamembertDialog(const QMap<QString, int>& data, QWidget *parent)
    : QDialog(parent), ressources(data)
{
    resize(500, 500);
    setWindowTitle("Ressources les plus utilisées");

    // Générer des couleurs aléatoires
    for (int i = 0; i < ressources.size(); ++i) {
        couleurs.append(QColor::fromRgb(
            QRandomGenerator::global()->bounded(255),
            QRandomGenerator::global()->bounded(255),
            QRandomGenerator::global()->bounded(255)
            ));
    }
}

void CamembertDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect(50, 50, 300, 300);
    int total = 0;
    for (int val : ressources.values()) total += val;

    int startAngle = 0;
    int i = 0;
    for (auto it = ressources.begin(); it != ressources.end(); ++it, ++i) {
        int span = qRound(360.0 * it.value() / total);
        painter.setBrush(couleurs[i]);
        painter.drawPie(rect, startAngle * 16, span * 16);
        startAngle += span;
    }

    // Légende
    int y = 370;
    i = 0;
    for (auto it = ressources.begin(); it != ressources.end(); ++it, ++i) {
        painter.setBrush(couleurs[i]);
        painter.drawRect(50, y, 15, 15);
        painter.drawText(70, y + 12, it.key() + " (" + QString::number(it.value()) + ")");
        y += 20;
    }
}
