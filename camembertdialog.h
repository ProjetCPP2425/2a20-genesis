#ifndef CAMEMBERTDIALOG_H
#define CAMEMBERTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QColor>

class CamembertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CamembertDialog(const QMap<QString, int>& data, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMap<QString, int> ressources;
    QList<QColor> couleurs;
};

#endif // CAMEMBERTDIALOG_H
