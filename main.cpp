#include "boutique.h"
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "Connection.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test=c.createConnection();
    MainWindow w;
    w.show();
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
    // Test afficher() function
    Boutique b;
    QSqlQueryModel *model = b.afficher();
    if (model->rowCount() > 0) {
        qDebug() << "Database successfully retrieved" << model->rowCount() << "rows.";
    } else {
        qDebug() << "No data found in database.";
    }
    return a.exec();

}
