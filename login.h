#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class LOGIN;
}


class LOGIN : public QDialog
{
    Q_OBJECT

public:
    explicit LOGIN(QWidget *parent = nullptr);
    ~LOGIN();
    bool verifySecretAnswer(const QString &nom, const QString &question, const QString &reponse);
    void updatePassword(const QString &nom, const QString &newPassword);
    void showPasswordResetDialog();


private slots:
    void on_btnLogin_clicked();
    void on_btnCreateAccount_clicked();
    void onShowPasswordToggled(bool checked);
private slots:
    void on_btnForgotPassword_clicked();

private:
    Ui::LOGIN *ui;
    MainWindow *mainWindow;
};

#endif // LOGIN_H
