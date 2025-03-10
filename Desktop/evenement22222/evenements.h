    #ifndef EVENEMENTS_H
#define EVENEMENTS_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include<QDate>
class evenement
{
    int id_ev;
    QString nom;
    QDate date_debut;
    QDate date_fin;
    QString lieu;
    int capacite;
    QString sponsors;
    float budget;

public :
    evenement();
    evenement(QString, QDate ,QDate ,QString ,int ,QString ,float);
    bool modifier(QString old_nom);

    int Getid() { return id_ev; }
    void Setid(int val) { id_ev = val; }
    QString Getnom() { return nom; }
    void Setnom(QString val) { nom = val; }
    QDate Getdate_debut() { return date_debut; }
    void Setdate_debut(QDate val) { date_debut = val; }
    QDate Getdate_fin() { return date_fin; }
    void Setdate_fin(QDate val) { date_fin = val; }
    QString Getlieu() { return lieu; }
    void Setlieu(QString val) { lieu = val; }
    int Getcapacite() { return capacite; }
    void Setcapacite(int val) { capacite = val; }
    QString Getsponsors() { return sponsors; }
    void Setsponsors(QString val) { sponsors = val; }
    float Getbudget() { return budget; }
    void Setbudget(float val) { budget = val; }

    bool supprimer(QString nom);
    bool modifier();
    bool ajouter();
    QSqlQueryModel* afficher();
    //bool supprimer(int);

};

#endif // EVENEMENTS_H
