#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "evenements.h"
#include <QDate>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    Connection c;
    bool test=c.createConnection();
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                                 QObject::tr("connection successful.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);

    }
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

    evenement event(6, "hello", QDate(2025, 7, 1), QDate(2025, 6, 2), "Paris", 500, "Brand X", 10000.0);

    if (event.ajouter()) {
        QMessageBox::information(nullptr, "Succès", "Événement ajouté avec succès !");
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout de l'événement.");
    }

    w.show();
    return a.exec();
}
