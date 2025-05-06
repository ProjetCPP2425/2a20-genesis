
#include "login.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createConnection();

    //MainWindow w;
    if (test) {
        //w.show();
        QMessageBox::information(nullptr, QObject::tr("Database is Open"),
                                 QObject::tr("Connection successful.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database is Not Open"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }



    LOGIN loginWindow;
    loginWindow.show();

    return a.exec();
}
