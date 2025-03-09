#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "evenements.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView4->setModel(e.afficher());  // Charge les événements dès le lancement
}


MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_pushbutton_ajouter_2_clicked()
{
    evenement e;
    ui->tableView4->setModel(e.afficher());
    e.Setnom(ui->lineedit_nom->text());
    e.Setdate_debut(ui->lineedit_date_debut->date());
    e.Setdate_fin(ui->lineedit_date_fin->date());
    e.Setbudget(ui->lineedit_budget->text().toFloat());
    e.Setcapacite(ui->lineedit_capacite->text().toInt());
    e.Setlieu(ui->lineedit_lieu->text());
    e.Setsponsors(ui->lineedit_sponsors->text());

    bool test = e.ajouter();
    if(test){
        QMessageBox::information(nullptr, QObject::tr("Ajout effectué"),
                                 QObject::tr("L'événement a été ajouté avec succès."), QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur d'ajout"),
                              QObject::tr("L'ajout de l'événement a échoué."), QMessageBox::Ok);
    }
}
*/
void MainWindow::on_pushbutton_ajouter_2_clicked()
{
    // Get the input values from the user interface
    QString eventName = ui->lineedit_nom->text();
    QDate startDate = ui->lineedit_date_debut->date();
    QDate endDate = ui->lineedit_date_fin->date();
    float budget = ui->lineedit_budget->text().toFloat();
    int capacity = ui->lineedit_capacite->text().toInt();
    QString venue = ui->lineedit_lieu->text();
    QString sponsors = ui->lineedit_sponsors->text();

    // Validate that all required fields are filled in
    if (eventName.isEmpty() || venue.isEmpty() || sponsors.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // Ensure venue contains both block number and name (e.g., "Block A")
    if (!venue.contains("bloc", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Lieu' doit inclure le numéro de bloc, par exemple 'bloc A'.");
        return;
    }

    // Validate that the capacity and budget are non-negative
    if (capacity < 0) {
        QMessageBox::warning(this, "Erreur", "La capacité ne peut pas être négative.");
        return;
    }

    if (budget < 0) {
        QMessageBox::warning(this, "Erreur", "Le budget ne peut pas être négatif.");
        return;
    }

    // Validate that the start date is not later than the end date
    if (startDate > endDate) {
        QMessageBox::warning(this, "Erreur", "La date de début ne peut pas être postérieure à la date de fin.");
        return;
    }

    // Set the values into the event object
    evenement e;
    e.Setnom(eventName);
    e.Setdate_debut(startDate);
    e.Setdate_fin(endDate);
    e.Setbudget(budget);
    e.Setcapacite(capacity);
    e.Setlieu(venue);
    e.Setsponsors(sponsors);

    // Refresh the table model before adding the event
    ui->tableView4->setModel(e.afficher());

    // Try to add the event
    bool test = e.ajouter();
    if(test) {
        QMessageBox::information(this, "Ajout effectué", "L'événement a été ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur d'ajout", "L'ajout de l'événement a échoué.");
    }
}


void MainWindow::on_pushButton_supprimer_clicked()
{
    QString event_name = ui->lineedit_nom->text();  // Get the event name to delete

    if (event_name.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Le nom de l'événement à supprimer doit être renseigné."), QMessageBox::Ok);
        return;
    }

    evenement e;

    // Call the supprimer function
    bool test = e.supprimer(event_name);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                                 QObject::tr("L'événement a été supprimé avec succès."), QMessageBox::Ok);
        ui->tableView4->setModel(e.afficher());  // Refresh table to reflect the changes
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de suppression"),
                              QObject::tr("La suppression de l'événement a échoué."), QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    QString old_nom = ui->lineedit_old_nom->text(); // Get the old event name
    qDebug() << "Old event name: " << old_nom;

    // Récupérer les valeurs des champs de saisie
    QString eventName = ui->lineedit_nom->text();
    QDate startDate = ui->lineedit_date_debut->date();
    QDate endDate = ui->lineedit_date_fin->date();
    float budget = ui->lineedit_budget->text().toFloat();
    int capacity = ui->lineedit_capacite->text().toInt();
    QString venue = ui->lineedit_lieu->text();
    QString sponsors = ui->lineedit_sponsors->text();

    // Contrôle de saisie
    if (eventName.isEmpty() || venue.isEmpty() || sponsors.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // Vérification que le champ 'Lieu' contient le mot 'bloc' (insensible à la casse)
    if (!venue.contains("bloc", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Lieu' doit inclure le numéro de bloc, par exemple 'bloc A'.");
        return;
    }

    // Vérification que la capacité et le budget ne sont pas négatifs
    if (capacity < 0) {
        QMessageBox::warning(this, "Erreur", "La capacité ne peut pas être négative.");
        return;
    }

    if (budget < 0) {
        QMessageBox::warning(this, "Erreur", "Le budget ne peut pas être négatif.");
        return;
    }

    // Vérification que la date de début n'est pas postérieure à la date de fin
    if (startDate > endDate) {
        QMessageBox::warning(this, "Erreur", "La date de début ne peut pas être postérieure à la date de fin.");
        return;
    }

    // Créer l'objet événement avec les nouvelles valeurs
    evenement e;
    e.Setnom(eventName);
    e.Setdate_debut(startDate);
    e.Setdate_fin(endDate);
    e.Setbudget(budget);
    e.Setcapacite(capacity);
    e.Setlieu(venue);
    e.Setsponsors(sponsors);

    qDebug() << "New event name: " << e.Getnom();
    qDebug() << "Date debut: " << e.Getdate_debut();
    qDebug() << "Date fin: " << e.Getdate_fin();
    qDebug() << "Budget: " << e.Getbudget();
    qDebug() << "Capacite: " << e.Getcapacite();
    qDebug() << "Lieu: " << e.Getlieu();
    qDebug() << "Sponsors: " << e.Getsponsors();

    // Essayer de modifier l'événement
    bool test = e.modifier(old_nom);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Modification réussie"),
                                 QObject::tr("L'événement a été modifié avec succès."), QMessageBox::Ok);
        ui->tableView4->setModel(e.afficher()); // Rafraîchir le tableau
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de modification"),
                              QObject::tr("La modification de l'événement a échoué."), QMessageBox::Ok);
    }
}
