#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {
        setCursor(Qt::PointingHandCursor); // Curseur interactif
        setText("Afficher");
        setStyleSheet("color: blue; text-decoration: underline;"); // Style lien
    }

signals:
    void clicked(); // Signal à émettre

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked(); // Émettre le signal lorsqu'on clique
    }
};

#endif // CLICKABLELABEL_H
