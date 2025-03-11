#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "locataire.h"
#include <QRegularExpression>
#include <QString>
#include <QProcess>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Charger les données au démarrage
    locataires l;
    QSqlQueryModel *model = l.aff();
    fillTableFromModel(model);




    pixmap = new QPixmap("C:/Users/youss/OneDrive/Desktop/Sch/ProjC++/7.png"); // Replace with the actual image path
    if (!pixmap->isNull()) {
        ui->label_7->setPixmap(pixmap->scaled(ui->label_7->size(), Qt::KeepAspectRatio));
    }

    // Set window title
    this->setWindowTitle("Locataires");

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



void MainWindow::on_ajtr_clicked()
{
    QString nom_loc = ui->nom->text();
    QString mail_loc = ui->mail->text();
    QDate date_inscrit = ui->dins->date();
    QDate date_fc = ui->dfc->date();
    QString statut = ui->st->currentText();
    QString mode_p = ui->mp->currentText();
    QString prix = ui->px->text();


    //Validate if fields are empty
    if (nom_loc.isEmpty() || mail_loc.isEmpty() || prix.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    //Validate name
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ ]{3,30}$");
    if (!nameRegex.match(nom_loc).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le nom doit contenir uniquement des lettres (3-30 caractères) !");
        return;
    }

    // Validate email (simple structure of user@domain.com)
    QRegularExpression mailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    if (!mailRegex.match(mail_loc).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "L'email doit être valide.");
        return;
    }



    //vald prix
    if (prix.length() < 0) {
        QMessageBox::warning(this, "Erreur", "prix doit etre valide !");
        return;
    }

    /*//Validate prix number
    QRegularExpression prixRegex("^[0-9]{8,15}$");
    if (!prixRegex.match(prix).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "prix non valide");
        return;
    }*/



    // create locataire object without setting ID
    locataires L(0, nom_loc, mail_loc, date_inscrit, date_fc, statut, mode_p, prix);

    bool test = L.aj();
    if (test)
    {
        locataires l;
        QSqlQueryModel *model = l.aff();
        fillTableFromModel(model);
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué.\n"
                                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    //ui->centralwidget->layout()->setCurrentIndex(0);

    //Clear inputs
    ui->nom->clear();
    ui->mail->clear();
    ui->px->clear();
}

void MainWindow::on_dlt_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();

    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un locataire à supprimer.");
        return;
    }

    QString idToDelete = ui->tableWidget->item(selectedRow, 0)->text();

    locataires l;
    if (l.supp(idToDelete.toInt())) {

        QMessageBox::information(this, "Succès", "locataire supprimée !");
        QSqlQueryModel *model = l.aff();
        fillTableFromModel(model);
    } else {
        QMessageBox::warning(this, "Erreur", "L'ID n'existe pas ou suppression échouée.");
    }
}


void MainWindow::on_mdf_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un locataire à modifier !");
        return;
    }

    QString id = ui->tableWidget->item(selectedRow, 0)->text();
    QString oldNom = ui->tableWidget->item(selectedRow, 1)->text();
    QString oldMail = ui->tableWidget->item(selectedRow, 2)->text();
    QDate oldDins = QDate::fromString(ui->tableWidget->item(selectedRow, 3)->text(), "yyyy-MM-dd");
    QDate oldDfc = QDate::fromString(ui->tableWidget->item(selectedRow, 4)->text(), "yyyy-MM-dd");
    QString oldStatut = ui->tableWidget->item(selectedRow, 5)->text();
    QString oldModeP = ui->tableWidget->item(selectedRow, 6)->text();
    QString oldPrix = ui->tableWidget->item(selectedRow, 7)->text();

    QString nom = ui->nom->text().trimmed().isEmpty() ? oldNom : ui->nom->text().trimmed();
    QString mail = ui->mail->text().trimmed().isEmpty() ? oldMail : ui->mail->text().trimmed();
    QDate dins = ui->dins->date().isValid() ? ui->dins->date() : oldDins;
    QDate dfc = ui->dfc->date().isValid() ? ui->dfc->date() : oldDfc;
    QString statut = ui->st->currentText().trimmed().isEmpty() ? oldStatut : ui->st->currentText().trimmed();
    QString modeP = ui->mp->currentText().trimmed().isEmpty() ? oldModeP : ui->mp->currentText().trimmed();
    QString prix = ui->px->text().trimmed().isEmpty() ? oldPrix : ui->px->text().trimmed();

    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ ]{3,50}$");
    QRegularExpression emailRegex("^[\\w.-]+@[\\w.-]+\\.[a-zA-Z]{2,6}$");
    QRegularExpression priceRegex("^[0-9]+(\\.[0-9]{1,2})?$");

    if (!nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le nom doit contenir uniquement des lettres et au moins 3 caractères.");
        return;
    }

    if (!emailRegex.match(mail).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "L'adresse mail doit être valide.");
        return;
    }

    if (!priceRegex.match(prix).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le prix doit être un nombre valide.");
        return;
    }

    if (statut.isEmpty() || modeP.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // Correct constructor with 8 parameters
    locataires l(id.toInt(), nom, mail, dins, dfc, statut, modeP, prix);

    // Update the locataire in the database
    if (!l.modif(id.toInt())) {
        QMessageBox::warning(this, "Erreur", "L'ID n'existe pas ou modification échouée.");
        return;
    }

    QMessageBox::information(this, "Succès", "Le locataire a été modifié avec succès.");
}


/*void MainWindow::on_pdf_clicked()
{
    // Demander où enregistrer le fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "locataires_data.pdf", "PDF Files (*.pdf)");

    if (filePath.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // Initialisation du PDF
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    int y = 100; // Position Y initiale pour l'écriture

    // Titre du document
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, y, "Liste des Locataires");
    y += 50;

    // Définition de la police
    painter.setFont(QFont("Arial", 10));

    // Dessiner les en-têtes de colonnes
    painter.drawText(50, y, "ID");
    painter.drawText(150, y, "Nom");
    painter.drawText(300, y, "Email");
    painter.drawText(500, y, "Date Inscription");
    painter.drawText(650, y, "Date Fin Contrat");
    painter.drawText(800, y, "Statut");
    painter.drawText(950, y, "Mode Paiement");
    painter.drawText(1100, y, "Prix");
    y += 30;

    // Boucle pour récupérer toutes les lignes du tableau
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        painter.drawText(50, y, ui->tableWidget->item(row, 0)->text());
        painter.drawText(150, y, ui->tableWidget->item(row, 1)->text());
        painter.drawText(300, y, ui->tableWidget->item(row, 2)->text());
        painter.drawText(500, y, ui->tableWidget->item(row, 3)->text());
        painter.drawText(650, y, ui->tableWidget->item(row, 4)->text());
        painter.drawText(800, y, ui->tableWidget->item(row, 5)->text());
        painter.drawText(950, y, ui->tableWidget->item(row, 6)->text());
        painter.drawText(1100, y, ui->tableWidget->item(row, 7)->text());
        y += 30; // Espacement entre les lignes
    }

    painter.end(); // Terminer l'écriture du PDF

    QMessageBox::information(this, "Export Réussi", "Le fichier PDF a été généré avec succès !");
}

void MainWindow::on_alerte_clicked()
{
    // Vérifier si une ligne est sélectionnée
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un locataire !");
        return;
    }

    // Récupérer les informations du locataire
    QString id = ui->tableWidget->item(selectedRow, 0)->text();
    QString nom = ui->tableWidget->item(selectedRow, 1)->text();
    QString mail = ui->tableWidget->item(selectedRow, 2)->text();
    QString prix = ui->tableWidget->item(selectedRow, 7)->text(); // Prix dû

    // Vérifier que l'email n'est pas vide
    if (mail.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "L'adresse e-mail du locataire est invalide !");
        return;
    }

    // Construire le message d'alerte
    QString subject = "Alerte: Paiement en retard";
    QString body = QString("Bonjour %1,\n\n"
                           "Nous vous informons que votre paiement de %2 TND est en retard.\n"
                           "Merci de régulariser votre situation dès que possible.\n\n"
                           "Cordialement,\n"
                           "Gestini Support")
                       .arg(nom, prix);

    // Appeler un script externe pour envoyer l'e-mail
    QString program = "python3"; // Ou "python" selon votre système
    QStringList arguments;
    arguments << "send_email.py" << mail << subject << body; // Nom du script + arguments

    QProcess *process = new QProcess(this);
    process->startDetached(program, arguments);

    QMessageBox::information(this, "Alerte Envoyée", "L'email d'alerte a été envoyé à " + mail);
}*/



MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_ajtr_clicked()
{
    ui->centralwidget->layout()->setCurrentIndex(0);

}*/

