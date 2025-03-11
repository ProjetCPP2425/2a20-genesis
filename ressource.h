#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <QString>

class Ressource
{
public:
    Ressource();
    Ressource(int id, QString nom, QString type, QString etat, int quantite, float prix, QString localisation, QString dateAchat, QString dateMaintenance);

    int getId() const;
    QString getNom() const;
    QString getType() const;
    QString getEtat() const;
    int getQuantite() const;
    float getPrix() const;
    QString getLocalisation() const;
    QString getDateAchat() const;
    QString getDateMaintenance() const;

    void setNom(QString nom);
    void setType(QString type);
    void setEtat(QString etat);
    void setQuantite(int quantite);
    void setPrix(float prix);
    void setLocalisation(QString localisation);
    void setDateAchat(QString dateAchat);
    void setDateMaintenance(QString dateMaintenance);

private:
    int id;
    QString nom;
    QString type;
    QString etat;
    int quantite;
    float prix;
    QString localisation;
    QString dateAchat;
    QString dateMaintenance;
};

#endif // RESSOURCE_H
