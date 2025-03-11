#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_valider, &QPushButton::clicked, this, &MainWindow::ajouterRessource);
    connect(ui->pushButton_modif, &QPushButton::clicked, this, &MainWindow::modifierRessource);
    connect(ui->pushButton_supp, &QPushButton::clicked, this, &MainWindow::supprimerRessource);
    afficherRessources();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afficherRessources()
{
    QSqlQuery query("SELECT NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM RESSOURCES");

    // Définir les colonnes
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels({"Nom", "Type", "État", "Quantité", "Prix", "Localisation", "Date Achat", "Date Maintenance"});

    ui->tableWidget->setRowCount(0); // Réinitialiser le tableau
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // NOMR
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // TYPER
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // ETATR
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // QUANTITER
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // PRIXR
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // LOCR
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // DAR
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString())); // DM
        row++;
    }
}


void MainWindow::ajouterRessource()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "La base de données n'est pas ouverte !");
        return;
    }

    QSqlQuery query(db);

    // On insère la colonne IDR en utilisant la séquence directement :
    query.prepare(R"(
        INSERT INTO RESSOURCES (IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM)
        VALUES (RESSOURCES_SEQ.NEXTVAL, :nom, :type, :etat, :quantite, :prix,
                :localisation,
                TO_DATE(:dateAchat, 'YYYY-MM-DD'),
                TO_DATE(:dateMaintenance, 'YYYY-MM-DD'))
    )");

    query.bindValue(":nom", ui->lineEdit_11->text());
    query.bindValue(":type", ui->lineEdit_12->text());
    query.bindValue(":etat", ui->lineEdit_10->text());
    query.bindValue(":quantite", ui->lineEdit_7->text().toInt());
    query.bindValue(":prix", ui->lineEdit_8->text().toFloat());
    query.bindValue(":localisation", ui->lineEdit_9->text());
    query.bindValue(":dateAchat", ui->dateEdit_3->date().toString("yyyy-MM-dd"));
    query.bindValue(":dateMaintenance", ui->dateEdit_4->date().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout de la ressource : " + query.lastError().text());
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }

    QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès");
    afficherRessources();
}

void MainWindow::modifierRessource()
{
    QSqlQuery query;
    query.prepare("UPDATE RESSOURCES SET TYPER=:type, ETATR=:etat, QUANTITER=:quantite, PRIXR=:prix, LOCR=:localisation, DAR=:dateAchat, DM=:dateMaintenance "
                  "WHERE NOMR=:nom");
    query.bindValue(":type", ui->lineEdit_12->text());
    query.bindValue(":etat", ui->lineEdit_10->text());
    bool ok;
    int quantite = ui->lineEdit_7->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide.");
        return;
    }
    query.bindValue(":quantite", quantite);

    bool ok2;
    float prix = ui->lineEdit_8->text().toFloat(&ok2);
    if (!ok2) {
        QMessageBox::warning(this, "Erreur", "Prix invalide.");
        return;
    }
    query.bindValue(":prix", prix);
    query.bindValue(":localisation", ui->lineEdit_9->text());
    query.bindValue(":dateAchat", ui->dateEdit_3->date().toString("yyyy-MM-dd"));
    query.bindValue(":dateMaintenance", ui->dateEdit_4->date().toString("yyyy-MM-dd"));
    query.bindValue(":nom", ui->lineEdit_11->text()); // Condition to identify the resource

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Ressource modifiée avec succès");
        afficherRessources();
    } else {
        QMessageBox::warning(this, "Erreur", query.lastError().text());
    }
}

void MainWindow::supprimerRessource()
{
    QSqlQuery query;
    query.prepare("DELETE FROM RESSOURCES WHERE NOMR=:nom");
    query.bindValue(":nom", ui->lineEdit_11->text());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Ressource supprimée avec succès");
        afficherRessources();
    } else {
        QMessageBox::warning(this, "Erreur", query.lastError().text());
    }
}
