#include "locataire.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDate>
#include <QString>

locataires::locataires()
{
    id_loc = 0;
    nom_loc = "";
    mail_loc = "";
    date_inscrit = QDate::currentDate();
    date_fc = QDate::currentDate();
    statut = "";
    mode_p = "";
    prix = "";
}

locataires::locataires(int id_loc,QString nom_loc,QString mail_loc,QDate date_inscrit,QDate date_fc,QString statut,QString mode_p,QString prix) {
    this->id_loc = id_loc;
    this->nom_loc = nom_loc;
    this->mail_loc = mail_loc;
    this->date_inscrit = date_inscrit;
    this->date_fc = date_fc;
    this->statut = statut;
    this->mode_p = mode_p;
    this->prix = prix;
}

bool locataires::aj(){
    QSqlQuery query;

    QString res = QString::number(id_loc);

    // Get the next available ID from a sequence
    query.prepare("SELECT NVL(MAX(id_loc), 0) + 1 FROM locataires");
    if (!query.exec() || !query.next()) {
        return false;  // If the query fails, return false
    }

    // Now we don't need 'newId' anymore, just directly execute the insertion query
    query.prepare("insert into locataires ( nom_loc, mail_loc, date_inscrit, date_fc, statut, mode_p, prix) "
                  "values ( :nom_loc, :mail_loc, :date_inscrit, :date_fc, :statut, :mode_p, :prix)");

   // query.bindValue(":id_loc", res);  // Using the result directly
    query.bindValue(":nom_loc", nom_loc);
    query.bindValue(":mail_loc", mail_loc);
    query.bindValue(":date_inscrit", date_inscrit);
    query.bindValue(":date_fc", date_fc);
    query.bindValue(":statut", statut);
    query.bindValue(":mode_p", mode_p);
    query.bindValue(":prix", prix);

    return query.exec(); // Execute the query to insert data
}

QSqlQueryModel* locataires::aff(){
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("select * from locataires");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("date_inscrit"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("date_fc"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("mode_p"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("prix"));

    // Add extra columns for update and delete
    model->insertColumn(model->columnCount());
    model->insertColumn(model->columnCount());
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Update"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Delete"));

    return model;
}

bool locataires::supp(int id_loc){
    QSqlQuery query;
    query.prepare("DELETE FROM locataires WHERE id_loc = :id_loc");
    query.bindValue(":id_loc", id_loc);

    return query.exec();
}

bool locataires::modif(int id_loc) {
    if (id_loc <= 0) {
        qDebug() << "ID doit etre positif.";
        return false;
    }

    QSqlQuery query;
    QString date_inscrit = this->date_inscrit.toString("yyyy-MM-dd");  // Convert date to string format
    QString date_fc = this->date_fc.toString("yyyy-MM-dd");            // Convert date to string format

    query.prepare("UPDATE locataires SET nom_loc = :nom_loc, mail_loc = :mail_loc, "
                  "date_inscrit = TO_DATE(:date_inscrit, 'YYYY-MM-DD'), date_fc = TO_DATE(:date_fc, 'YYYY-MM-DD'), "
                  "statut = :statut, mode_p = :mode_p, prix = :prix WHERE id_loc = :id_loc");

    query.bindValue(":nom_loc", nom_loc);
    query.bindValue(":mail_loc", mail_loc);
    query.bindValue(":date_inscrit", date_inscrit);
    query.bindValue(":date_fc", date_fc);
    query.bindValue(":statut", statut);
    query.bindValue(":mode_p", mode_p);
    query.bindValue(":prix", prix);
    query.bindValue(":id_loc", id_loc);

    if (query.exec()) {
        qDebug() << "Locataire avec ID" << id_loc << "modifié avec succès !";
        return true;
    } else {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return false;
    }
}
