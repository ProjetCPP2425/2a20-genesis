#include "evenements.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QSqlError>
#include <QDate>

evenement::evenement(int id, QString nom, QDate date_debut, QDate date_fin , QString lieu, int capacite , QString sponsors, float budget ) {
    this->id=id;
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

    query.prepare("INSERT INTO IMENE.EVENEMENTS (ID, NOM, DATE_DEBUT, DATE_FIN, LIEU, CAPACITE, SPONSORS, BUDGET)"
                  "VALUES (:id, :nom, :date_debut, :date_fin, :lieu, :capacite, :sponsors , :budget)");

    query.bindValue(":id" , id);
    query.bindValue(":nom" , nom);
    query.bindValue(":date_debut" , date_debut);
    query.bindValue(":date_fin" , date_fin);
    query.bindValue(":lieu" , lieu);
    query.bindValue(":capacite" , capacite);
    query.bindValue(":sponsors" , sponsors);
    query.bindValue(":budget" , budget);
    // Execute the query
    if (!query.exec())
    {
        // Log the error message if the query fails
        qDebug() << "Insert query failed:" << query.lastError().text();
        return false; // Return false if insertion fails
    }

    return true; // Return true if successful


    return query.exec();
}
