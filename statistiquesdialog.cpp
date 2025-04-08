#include <QSqlQuery>
#include <QMessageBox>
#include "statistiquesdialog.h"  // Pour inclure la déclaration de la classe StatistiquesDialog
#include "ui_statistiquesdialog.h"  // Pour inclure la définition générée de l'UI


StatistiquesDialog::StatistiquesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatistiquesDialog)
{
    ui->setupUi(this);  // Initialiser l'UI
    afficherStatistiques();  // Afficher les statistiques dès l'ouverture de la boîte de dialogue
}

StatistiquesDialog::~StatistiquesDialog()
{
    delete ui;  // Libérer l'UI à la destruction de la boîte de dialogue
}

void StatistiquesDialog::afficherStatistiques()
{
    // Requête pour calculer le prix moyen des ressources
    QSqlQuery queryPrix;
    queryPrix.prepare("SELECT AVG(PRIXR) FROM RESSOURCES");
    if (queryPrix.exec()) {
        if (queryPrix.next()) {
            float prixMoyen = queryPrix.value(0).toFloat();
            ui->label_prix->setText("Prix moyen des ressources : " + QString::number(prixMoyen));
        }
    }

    // Requête pour obtenir les ressources les plus utilisées par localisation
    QSqlQuery queryLocalisation;
    queryLocalisation.prepare("SELECT LOCR, COUNT(*) FROM RESSOURCES GROUP BY LOCR ORDER BY COUNT(*) DESC LIMIT 1");
    if (queryLocalisation.exec()) {
        if (queryLocalisation.next()) {
            QString localisation = queryLocalisation.value(0).toString();
            ui->label_localisation->setText("Ressource la plus utilisée par localisation : " + localisation);
        }
    }
}
