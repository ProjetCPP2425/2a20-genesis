#ifndef BOUTIQUE_H
#define BOUTIQUE_H

#include <QString>
#include <QSqlQueryModel>

class Boutique
{
private:
    QString nom;
    QString statut;
    QString localisation;
    QString contact;
    QString categorie;

public:
    // Constructeurs
    Boutique();
    Boutique(QString nom,
             QString statut,
             QString localisation,
             QString contact,
             QString categorie);

    // Getters / Setters
    QString getNom() const;
    void setNom(const QString &value);

    QString getStatut() const;
    void setStatut(const QString &value);

    QString getLocalisation() const;
    void setLocalisation(const QString &value);

    QString getContact() const;
    void setContact(const QString &value);

    QString getCategorie() const;
    void setCategorie(const QString &value);

    // CRUD
    bool ajouter();                             // INSERT (sans ID_B)
    QSqlQueryModel* afficher();                 // SELECT
    bool supprimer(const QString &idToDelete);  // DELETE
    bool modifier(const QString &idToUpdate);   // UPDATE
};

#endif // BOUTIQUE_H
