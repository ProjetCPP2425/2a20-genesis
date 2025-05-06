#ifndef ETUDIANT_H
#define ETUDIANT_H
#include<QSqlQuery>
#include<QSqlQueryModel>

class ETUDIANT
{
public:
    ETUDIANT();

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer (int);
};

#endif // ETUDIANT_H
