#include "Connection.h"
#include <QDebug>
Connection::Connection()
{

}

bool Connection::createConnection()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projetcpp");//inserer le nom de la source de données
    db.setUserName("Youssef");//inserer nom de l'utilisateur
    db.setPassword("Bahdoudi");//inserer mot de passe de cet utilisateur
    qDebug() << "unable to connect";
    if (db.open())
        test=true;

    return  test;
}
void Connection::closeConnection(){db.close();}
