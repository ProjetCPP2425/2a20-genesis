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
    ui->tableView4->setModel(e.afficher());

    connect(ui->tableView4->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);
}

void MainWindow::onRowSelected()
{
    QModelIndexList selectedIndexes = ui->tableView4->selectionModel()->selectedRows();

    if (!selectedIndexes.isEmpty()) {
        QModelIndex index = selectedIndexes.first();

        QString eventName = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 1)).toString();
        QDate startDate = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 2)).toDate();
        QDate endDate = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 3)).toDate();
        QString venue = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 4)).toString();
        int capacity = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 5)).toInt();
        QString sponsors = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 6)).toString();
        float budget = ui->tableView4->model()->data(ui->tableView4->model()->index(index.row(), 7)).toFloat();

        ui->lineedit_nom->setText(eventName);
        ui->lineedit_date_debut->setDate(startDate);
        ui->lineedit_date_fin->setDate(endDate);
        ui->lineedit_lieu->setText(venue);
        ui->lineedit_capacite->setText(QString::number(capacity));
        ui->lineedit_sponsors->setText(sponsors);
        ui->lineedit_budget->setText(QString::number(budget));
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushbutton_ajouter_2_clicked()
{
    QString eventName = ui->lineedit_nom->text();
    QDate startDate = ui->lineedit_date_debut->date();
    QDate endDate = ui->lineedit_date_fin->date();
    float budget = ui->lineedit_budget->text().toFloat();
    int capacity = ui->lineedit_capacite->text().toInt();
    QString venue = ui->lineedit_lieu->text();
    QString sponsors = ui->lineedit_sponsors->text();

    if (eventName.isEmpty() || venue.isEmpty() || sponsors.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    if (!venue.contains("bloc", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Lieu' doit inclure le numéro de bloc, par exemple 'bloc A'.");
        return;
    }

    if (capacity < 0) {
        QMessageBox::warning(this, "Erreur", "La capacité ne peut pas être négative.");
        return;
    }

    if (budget < 0) {
        QMessageBox::warning(this, "Erreur", "Le budget ne peut pas être négatif.");
        return;
    }

    if (startDate < endDate) {
        QMessageBox::warning(this, "Erreur", "La date de début ne peut pas être postérieure à la date de fin.");
        return;
    }

    evenement e;
    e.Setnom(eventName);
    e.Setdate_debut(startDate);
    e.Setdate_fin(endDate);
    e.Setbudget(budget);
    e.Setcapacite(capacity);
    e.Setlieu(venue);
    e.Setsponsors(sponsors);

    // Ajouter l'événement à la base de données
    bool test = e.ajouter();
    if (test) {
        QMessageBox::information(this, "Ajout effectué", "L'événement a été ajouté avec succès.");
        ui->tableView4->setModel(e.afficher()); // Rafraîchir le tableau
        connect(ui->tableView4->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MainWindow::onRowSelected);
    } else {
        QMessageBox::critical(this, "Erreur d'ajout", "L'ajout de l'événement a échoué.");
    }
}




void MainWindow::on_pushButton_supprimer_clicked()
{
    QString event_name = ui->lineedit_old_nom->text();

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
        ui->tableView4->setModel(e.afficher());
        connect(ui->tableView4->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MainWindow::onRowSelected);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de suppression"),
                              QObject::tr("La suppression de l'événement a échoué."), QMessageBox::Ok);
    }
}


void MainWindow::on_pushButton_modifier_clicked()
{
    //QString old_nom = ui->lineedit_old_nom->text();
    QModelIndexList selectedIndexes = ui->tableView4->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à modifier.");
        return;
    }

    QString old_nom = ui->tableView4->model()->data(ui->tableView4->model()->index(selectedIndexes.first().row(), 1)).toString();

    qDebug() << "Old event name: " << old_nom;

    QString eventName = ui->lineedit_nom->text();
    QDate startDate = ui->lineedit_date_debut->date();
    QDate endDate = ui->lineedit_date_fin->date();
    float budget = ui->lineedit_budget->text().toFloat();
    int capacity = ui->lineedit_capacite->text().toInt();
    QString venue = ui->lineedit_lieu->text();
    QString sponsors = ui->lineedit_sponsors->text();

    if (eventName.isEmpty() || venue.isEmpty() || sponsors.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    if (!venue.contains("bloc", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Lieu' doit inclure le numéro de bloc, par exemple 'bloc A'.");
        return;
    }

    if (capacity < 0) {
        QMessageBox::warning(this, "Erreur", "La capacité ne peut pas être négative.");
        return;
    }

    if (budget < 0) {
        QMessageBox::warning(this, "Erreur", "Le budget ne peut pas être négatif.");
        return;
    }

    if (startDate < endDate) {
        QMessageBox::warning(this, "Erreur", "La date de début ne peut pas être postérieure à la date de fin.");
        return;
    }

    evenement e;
    e.Setnom(eventName);
    e.Setdate_debut(startDate);
    e.Setdate_fin(endDate);
    e.Setbudget(budget);
    e.Setcapacite(capacity);
    e.Setlieu(venue);
    e.Setsponsors(sponsors);

    qDebug() << "Nom: " << e.Getnom();
    qDebug() << "Date debut: " << e.Getdate_debut();
    qDebug() << "Date fin: " << e.Getdate_fin();
    qDebug() << "Budget: " << e.Getbudget();
    qDebug() << "Capacite: " << e.Getcapacite();
    qDebug() << "Lieu: " << e.Getlieu();
    qDebug() << "Sponsors: " << e.Getsponsors();

    bool test = e.modifier(old_nom);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Modification réussie"),
                                 QObject::tr("L'événement a été modifié avec succès."), QMessageBox::Ok);
        ui->tableView4->setModel(e.afficher()); // Rafraîchir le tableau
        connect(ui->tableView4->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MainWindow::onRowSelected);

    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de modification"),
                              QObject::tr("La modification de l'événement a échoué."), QMessageBox::Ok);
    }
}
