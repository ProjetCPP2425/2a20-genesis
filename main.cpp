#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "Connection.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;

    qDebug() << "Tentative de connexion à la base de données...";

    if (c.createConnection())
    {
        qDebug() << "Connexion réussie.";
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                 QObject::tr("Connection successful.\n"
                                             "Click OK to continue."),
                                 QMessageBox::Ok);

        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        qDebug() << "Échec de la connexion.";
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\n"
                                          "Click OK to exit."),
                              QMessageBox::Ok);
        return 1;
    }
}
