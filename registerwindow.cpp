#include "registerwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

RegisterWindow::RegisterWindow(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLineEdit *userEdit = new QLineEdit(this);
    QLineEdit *passEdit = new QLineEdit(this);
    passEdit->setEchoMode(QLineEdit::Password);
    QPushButton *btn = new QPushButton("Register", this);

    layout->addWidget(userEdit);
    layout->addWidget(passEdit);
    layout->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, &RegisterWindow::registerUser);
}

void RegisterWindow::registerUser()
{
    emit userRegistered("username", "password"); // Adaptez avec les vrais champs
    accept();
}
