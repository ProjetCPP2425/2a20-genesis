#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>

class RegisterWindow : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterWindow(QWidget *parent = nullptr);

signals:
    void userRegistered(const QString &user, const QString &pass);

private slots:
    void registerUser();
};

#endif // REGISTERWINDOW_H
