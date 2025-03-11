#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boutique.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QPixmap>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Chargement des images
    QPixmap pixmap1("C:/Users/lenovo/Desktop/Boutique/kharita.jpeg");
    if (!pixmap1.isNull()) {
        ui->label_8->setPixmap(pixmap1.scaled(ui->label_8->size(), Qt::KeepAspectRatio));
    }

    QPixmap pixmap2("C:/Users/lenovo/Desktop/Boutique/logo.png");
    if (!pixmap2.isNull()) {
        ui->label_6->setPixmap(pixmap2.scaled(ui->label_6->size(), Qt::KeepAspectRatio));
    }

    QPixmap pixmap3("C:/Users/lenovo/Desktop/Boutique/stat.png");
    if (!pixmap3.isNull()) {
        ui->label_7->setPixmap(pixmap3.scaled(ui->label_7->size(), Qt::KeepAspectRatio));
    }

    this->setWindowTitle("Gestion des Boutiques");

    // Charger les données au démarrage
    Boutique b;
    QSqlQueryModel *model = b.afficher();
    fillTableFromModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillTableFromModel(QSqlQueryModel *model)
{
    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount());

    for (int c = 0; c < model->columnCount(); ++c) {
        ui->tableWidget->setHorizontalHeaderItem(c, new QTableWidgetItem(model->headerData(c, Qt::Horizontal).toString()));
    }

    for (int r = 0; r < model->rowCount(); ++r) {
        for (int c = 0; c < model->columnCount(); ++c) {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem(model->data(model->index(r, c)).toString()));
        }
    }
}

/* ------------------------------------------------------
   🔹 Ajouter une boutique (Sans entrer ID_B car auto-incrémenté)
   ------------------------------------------------------ */
void MainWindow::on_ajouter_clicked()
{
    // 🔹 Retrieve input values
    QString nom          = ui->lineEdit->text().trimmed();
    QString contact      = ui->lineEdit_3->text().trimmed();
    QString statut       = ui->lineEdit_4->text().trimmed();
    QString localisation = ui->lineEdit_5->text().trimmed();
    QString categorie    = ui->lineEdit_6->text().trimmed();

    // 🔹 Validate if fields are empty
    if (nom.isEmpty() || contact.isEmpty() || statut.isEmpty() || localisation.isEmpty() || categorie.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    // 🔹 Validate name (only letters, 3-30 characters)
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ ]{3,30}$");
    if (!nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le nom doit contenir uniquement des lettres (3-30 caractères) !");
        return;
    }

    // 🔹 Validate contact (only numbers, 8-15 digits)
    QRegularExpression phoneRegex("^[0-9]{8,15}$");
    if (!phoneRegex.match(contact).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le contact doit être un numéro valide (8-15 chiffres) !");
        return;
    }

    // 🔹 Validate statut (must be "actif" or "inactif")
    if (statut.toLower() != "actif" && statut.toLower() != "inactif") {
        QMessageBox::warning(this, "Erreur", "Le statut doit être 'actif' ou 'inactif' !");
        return;
    }

    // 🔹 Validate localisation (3-50 characters)
    if (localisation.length() < 3 || localisation.length() > 50) {
        QMessageBox::warning(this, "Erreur", "La localisation doit contenir entre 3 et 50 caractères !");
        return;
    }

    // 🔹 Validate catégorie (3-20 characters)
    if (categorie.length() < 3 || categorie.length() > 20) {
        QMessageBox::warning(this, "Erreur", "La catégorie doit contenir entre 3 et 20 caractères !");
        return;
    }

    // ✅ If all validations pass, create and insert boutique
    Boutique b(nom, statut, localisation, contact, categorie);
    if (b.ajouter()) {
        QMessageBox::information(this, "Succès", "Boutique ajoutée avec succès !");

        // 🔄 Clear input fields after successful insertion
        ui->lineEdit->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit_6->clear();

        // 🔄 Refresh table
        QSqlQueryModel *model = b.afficher();
        fillTableFromModel(model);
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de la boutique !");
    }
}

/* ------------------------------------------------------
   🔹 Supprimer une boutique (Sélection depuis le tableau)
   ------------------------------------------------------ */
void MainWindow::on_supprimer_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();

    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une boutique à supprimer.");
        return;
    }

    QString idToDelete = ui->tableWidget->item(selectedRow, 0)->text();

    Boutique b;
    if (b.supprimer(idToDelete)) {
        QMessageBox::information(this, "Succès", "Boutique supprimée !");
        QSqlQueryModel *model = b.afficher();
        fillTableFromModel(model);
    } else {
        QMessageBox::warning(this, "Erreur", "L'ID n'existe pas ou suppression échouée.");
    }
}

/* ------------------------------------------------------
   🔹 Modifier une boutique (Sélection depuis le tableau)
   ------------------------------------------------------ */
void MainWindow::on_modifier_clicked()
{
    // Vérifier si une ligne est sélectionnée
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une boutique à modifier !");
        return;
    }

    // Récupération de l'ID de la boutique sélectionnée
    QString id = ui->tableWidget->item(selectedRow, 0)->text();

    // Récupérer les anciennes valeurs (si l'utilisateur ne les modifie pas)
    QString oldNom = ui->tableWidget->item(selectedRow, 1)->text();
    QString oldStatut = ui->tableWidget->item(selectedRow, 2)->text();
    QString oldLocalisation = ui->tableWidget->item(selectedRow, 3)->text();
    QString oldContact = ui->tableWidget->item(selectedRow, 4)->text();
    QString oldCategorie = ui->tableWidget->item(selectedRow, 5)->text();

    // Récupérer les nouvelles valeurs (si l'utilisateur a modifié un champ)
    QString nom = ui->lineEdit->text().trimmed().isEmpty() ? oldNom : ui->lineEdit->text().trimmed();
    QString contact = ui->lineEdit_3->text().trimmed().isEmpty() ? oldContact : ui->lineEdit_3->text().trimmed();
    QString statut = ui->lineEdit_4->text().trimmed().isEmpty() ? oldStatut : ui->lineEdit_4->text().trimmed();
    QString localisation = ui->lineEdit_5->text().trimmed().isEmpty() ? oldLocalisation : ui->lineEdit_5->text().trimmed();
    QString categorie = ui->lineEdit_6->text().trimmed().isEmpty() ? oldCategorie : ui->lineEdit_6->text().trimmed();

    // **Contrôle de saisie**
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ ]{3,50}$"); // Nom: lettres et espaces uniquement
    QRegularExpression phoneRegex("^[0-9]{8,15}$"); // Contact: uniquement des chiffres (8-15 chiffres)

    if (!nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le nom doit contenir uniquement des lettres et au moins 3 caractères.");
        return;
    }

    if (!phoneRegex.match(contact).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le contact doit être un numéro valide (8-15 chiffres).");
        return;
    }

    if (statut.isEmpty() || localisation.isEmpty() || categorie.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs correctement !");
        return;
    }

    // Créer l'objet avec les valeurs mises à jour
    Boutique b(nom, statut, localisation, contact, categorie);

    // Exécuter la mise à jour dans la base de données
    if (!b.modifier(id)) {
        QMessageBox::warning(this, "Erreur", "L'ID n'existe pas ou modification échouée.");
        return;
    }

    QMessageBox::information(this, "Succès", "Boutique modifiée avec succès !");

    // Rafraîchir la table
    QSqlQueryModel *model = b.afficher();
    fillTableFromModel(model);

    // Effacer les champs après modification
    ui->lineEdit->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
}
