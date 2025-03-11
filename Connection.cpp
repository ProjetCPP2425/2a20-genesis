#include "Connection.h"
#include<QDebug>

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projetcpp");//inserer le nom de la source de données
db.setUserName("Youssef");//inserer nom de l'utilisateur
db.setPassword("Bahdoudi");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
//void Connection::closeconnect(){db.close();}
