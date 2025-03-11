#include "boutique.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

// Constructeur par défaut
Boutique::Boutique()
{
    nom = "";
    statut = "";
    localisation = "";
    contact = "";
    categorie = "";
}

// Constructeur avec paramètres
Boutique::Boutique(QString nom, QString statut, QString localisation, QString contact, QString categorie)
{
    this->nom = nom;
    this->statut = statut;
    this->localisation = localisation;
    this->contact = contact;
    this->categorie = categorie;
}

// Ajouter une boutique (ID_B est auto-incrémenté)
bool Boutique::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO BOUTIQUE (NOM_B, STATUT_B, LOCALISATION, CONTACT, CATEGORIE) "
                  "VALUES (:NOM_B, :STATUT_B, :LOCALISATION, :CONTACT, :CATEGORIE)");

    query.bindValue(":NOM_B", nom);
    query.bindValue(":STATUT_B", statut);
    query.bindValue(":LOCALISATION", localisation);
    query.bindValue(":CONTACT", contact);
    query.bindValue(":CATEGORIE", categorie);

    if (!query.exec()) {
        QString errorMsg = "Erreur lors de l'ajout: " + query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur SQL", errorMsg);
        return false;
    }

    return true;
}

// Afficher les boutiques
QSqlQueryModel* Boutique::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_B, NOM_B, STATUT_B, LOCALISATION, CONTACT, CATEGORIE FROM BOUTIQUE");

    if (model->lastError().isValid()) {
        qDebug() << "Error fetching data:" << model->lastError().text();
    }

    return model;
}


// Supprimer une boutique par ID
bool Boutique::supprimer(const QString &idToDelete)
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_B FROM BOUTIQUE WHERE ID_B = :idToDelete");
    checkQuery.bindValue(":idToDelete", idToDelete);
    checkQuery.exec();

    if (!checkQuery.next()) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID n'existe pas. Veuillez entrer un ID valide.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM BOUTIQUE WHERE ID_B = :idToDelete");
    query.bindValue(":idToDelete", idToDelete);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", "Échec de la suppression: " + query.lastError().text());
        return false;
    }

    return true;
}

// Modifier une boutique
bool Boutique::modifier(const QString &idToUpdate)
{
    QSqlQuery query;
    query.prepare("UPDATE BOUTIQUE SET "
                  "NOM_B = :NOM_B, "
                  "STATUT_B = :STATUT_B, "
                  "LOCALISATION = :LOCALISATION, "
                  "CONTACT = :CONTACT, "
                  "CATEGORIE = :CATEGORIE "
                  "WHERE ID_B = :ID_B");

    query.bindValue(":NOM_B", nom);
    query.bindValue(":STATUT_B", statut);
    query.bindValue(":LOCALISATION", localisation);
    query.bindValue(":CONTACT", contact);
    query.bindValue(":CATEGORIE", categorie);
    query.bindValue(":ID_B", idToUpdate);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        return false;
    }
    return true;
}
