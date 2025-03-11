#include "evenements.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>
#include "evenements.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

evenement::evenement() {
    nom = "";
    date_debut = QDate();
    date_fin = QDate();
    lieu = "";
    capacite = 0;
    sponsors = "";
    budget = 0.0f;
}

evenement::evenement(QString nom, QDate date_debut, QDate date_fin , QString lieu, int capacite , QString sponsors, float budget ) {
    this->nom=nom;
    this->date_debut=date_debut;
    this->date_fin=date_fin;
    this->lieu=lieu;
    this->capacite=capacite;
    this->sponsors=sponsors;
    this->budget=budget;
}
bool evenement::ajouter(){
    QSqlQuery query;

    query.prepare("INSERT INTO IMENE.EVENEMENTS (NOM, DATE_DEBUT, DATE_FIN, LIEU, CAPACITE, SPONSORS, BUDGET)"
                  "VALUES (:nom, :date_debut, :date_fin, :lieu, :capacite, :sponsors , :budget)");

    query.bindValue(":nom" , nom);
    query.bindValue(":date_debut" , date_debut);
    query.bindValue(":date_fin" , date_fin);
    query.bindValue(":lieu" , lieu);
    query.bindValue(":capacite" , capacite);
    query.bindValue(":sponsors" , sponsors);
    query.bindValue(":budget" , budget);

    if (!query.exec())
    {
        qDebug() << "Insert query failed:" << query.lastError().text();
        return false;
    }

    return true;


    //return query.exec();
}
bool evenement::modifier(QString old_nom)
{
    QSqlQuery query;
    query.prepare("UPDATE evenements SET nom=:nom, date_debut=:date_debut, date_fin=:date_fin, budget=:budget, capacite=:capacite, lieu=:lieu, sponsors=:sponsors WHERE nom=:old_nom");

    query.bindValue(":old_nom", old_nom);
    query.bindValue(":nom", nom);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":budget", budget);
    query.bindValue(":capacite", capacite);
    query.bindValue(":lieu", lieu);
    query.bindValue(":sponsors", sponsors);

    return query.exec();
}



QSqlQueryModel* evenement::afficher(){
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("SELECT * FROM IMENE.EVENEMENTS");

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID_EV"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("DATE_DEBUT"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("DATE_FIN"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("LIEU"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("CAPACITE"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("SPONSORS"));
    model->setHeaderData(7,Qt::Horizontal,QObject::tr("BUDGET"));

    return model;
}

bool evenement::supprimer(QString nom) {
    QSqlQuery query;

    query.prepare("DELETE FROM IMENE.EVENEMENTS WHERE NOM = :nom");

    query.bindValue(":nom", nom);

    if (!query.exec()) {
        qDebug() << "Delete query failed:" << query.lastError().text();
        return false;
    }

    return true;
}

