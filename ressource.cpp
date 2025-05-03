#include "ressource.h"

Ressource::Ressource() {}

Ressource::Ressource(int id, QString nom, QString type, QString etat, int quantite, float prix, QString localisation, QString dateAchat, QString dateMaintenance)
    : id(id), nom(nom), type(type), etat(etat), quantite(quantite), prix(prix), localisation(localisation), dateAchat(dateAchat), dateMaintenance(dateMaintenance) {}

int Ressource::getId() const { return id; }
QString Ressource::getNom() const { return nom; }
QString Ressource::getType() const { return type; }
QString Ressource::getEtat() const { return etat; }
int Ressource::getQuantite() const { return quantite; }
float Ressource::getPrix() const { return prix; }
QString Ressource::getLocalisation() const { return localisation; }
QString Ressource::getDateAchat() const { return dateAchat; }
QString Ressource::getDateMaintenance() const { return dateMaintenance; }

void Ressource::setNom(QString nom) { this->nom = nom; }
void Ressource::setType(QString type) { this->type = type; }
void Ressource::setEtat(QString etat) { this->etat = etat; }
void Ressource::setQuantite(int quantite) { this->quantite = quantite; }
void Ressource::setPrix(float prix) { this->prix = prix; }
void Ressource::setLocalisation(QString localisation) { this->localisation = localisation; }
void Ressource::setDateAchat(QString dateAchat) { this->dateAchat = dateAchat; }
void Ressource::setDateMaintenance(QString dateMaintenance) { this->dateMaintenance = dateMaintenance; }
