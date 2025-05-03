#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boutique.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QPixmap>
#include <QRegularExpression>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boutique.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QPixmap>
#include <QRegularExpression>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QTextStream>
#include <QPageSize>
#include "statistique.h"
#include <QTextToSpeech>
#include <QInputDialog>
#include <QComboBox>
#include <QDate>
#include <QTime>
#include <QLocale>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include<QMap>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QScrollArea>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlError>
#include <QJsonArray>
#include <QJsonValue>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->localisation, &QPushButton::clicked, this, &MainWindow::afficherCarteInteractive);
    QPixmap pixmap2("C:/Users/lenovo/Desktop/Boutique/logo.png");
    if (!pixmap2.isNull()) {
        ui->label_6->setPixmap(pixmap2.scaled(ui->label_6->size(), Qt::KeepAspectRatio));
    }
    this->setWindowTitle("Gestion des Boutiques");

    // Charger les données au démarrage
    Boutique b;
    QSqlQueryModel *model = b.afficher();
    fillTableFromModel(model);


    arduino = new QSerialPort(this);
    arduino->setPortName("COM3"); // ⚠️ adapte au vrai port
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    if (arduino->open(QIODevice::ReadWrite)) {
        connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readFromArduino);
    } else {
        ui->label->setText("Échec de connexion Arduino");
    }


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
void MainWindow::on_tri_clicked()
{
    QString triValue = ui->comboBox->currentText().trimmed().toLower();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    if (triValue == "tous") {
        query.prepare("SELECT * FROM BOUTIQUE ORDER BY STATUT_B DESC, NOM_B ASC");
    }
    else if (triValue == "actif") {
        query.prepare("SELECT * FROM BOUTIQUE ORDER BY CASE WHEN STATUT_B = 'actif' THEN 0 ELSE 1 END, NOM_B ASC");
    }
    else if (triValue == "inactif") {
        query.prepare("SELECT * FROM BOUTIQUE ORDER BY CASE WHEN STATUT_B = 'inactif' THEN 0 ELSE 1 END, NOM_B ASC");
    }
    else {
        query.prepare("SELECT * FROM BOUTIQUE WHERE LOWER(STATUT_B) = :val ORDER BY NOM_B ASC");
        query.bindValue(":val", triValue);
    }

    query.exec();
    model->setQuery(query);
    fillTableFromModel(model);
}

void MainWindow::on_chercher_clicked()
{
    QString search = ui->lineEdit_7->text().trimmed();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM BOUTIQUE WHERE LOWER(NOM_B) LIKE :val OR LOWER(CONTACT) LIKE :val OR LOWER(STATUT_B) LIKE :val OR LOWER(LOCALISATION) LIKE :val OR LOWER(CATEGORIE) LIKE :val");
    query.bindValue(":val", "%" + search.toLower() + "%");
    query.exec();
    model->setQuery(query);
    fillTableFromModel(model);
}
void MainWindow::on_pdf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter;
    if (!painter.begin(&writer)) {
        QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    // 🟤 Fond beige clair
    painter.fillRect(QRectF(0, 0, writer.width(), writer.height()), QColor("#f5ede0"));

    // 🔵 Titre avec fond bleu
    QFont titleFont("Arial", 26, QFont::Bold);
    painter.setFont(titleFont);
    int titleWidth = 1200;
    int titleHeight = 100;
    int titleX = (writer.width() - titleWidth) / 2;
    QRect titleRect(titleX, 40, titleWidth, titleHeight);

    painter.setBrush(QColor("#2f466c"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(titleRect, 15, 15);
    painter.setPen(Qt::white);
    painter.drawText(titleRect, Qt::AlignCenter, "🗂️ Liste des Boutiques");

    // 🟦 Paramètres du tableau
    int tableTotalWidth = 1600;
    int startX = (writer.width() - tableTotalWidth) / 2;
    int startY = 180;
    int rowHeight = 80; // Increased row height
    // Original column widths maintained
    int columnWidths[] = {120, 300, 180, 300, 250, 300};

    // En-têtes - EXACTLY AS IN YOUR ORIGINAL CODE
    QFont headerFont("Arial", 11, QFont::Bold);
    painter.setFont(headerFont);
    painter.setPen(Qt::white);
    QStringList headers = {"ID", "Nom", "Statut", "Localisation", "Contact", "Catégorie"};

    // Dessiner les en-têtes - NO CHANGES TO HEADERS
    int currentX = startX;
    for (int i = 0; i < headers.size(); ++i) {
        QRect rect(currentX, startY, columnWidths[i], rowHeight);
        painter.setBrush(QColor("#2f466c"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);
        painter.setPen(Qt::white);
        painter.drawText(rect, Qt::AlignCenter, headers[i]);
        currentX += columnWidths[i];
    }

    // 📄 Données - ONLY IMPROVED SPACING FOR CONTENT
    QFont contentFont("Arial", 10);
    painter.setFont(contentFont);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        currentX = startX;
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString text = item ? item->text() : "";

            QRect rect(currentX, startY + (row + 1) * rowHeight, columnWidths[col], rowHeight);

            // Couleurs alternées pour les lignes
            painter.setBrush(row % 2 ? QColor("#ffffff") : QColor("#f0f0f0"));
            painter.setPen(QColor("#d0d0d0"));
            painter.drawRect(rect);

            // Improved text spacing - ONLY CHANGE WAS HERE
            painter.setPen(Qt::black);
            QRect textRect = rect.adjusted(10, 10, -10, -10); // More padding
            QTextOption option;
            option.setAlignment(Qt::AlignCenter);
            option.setWrapMode(QTextOption::WordWrap);
            painter.drawText(textRect, text, option);

            currentX += columnWidths[col];
        }
    }

    // Ajouter un pied de page avec la date
    QString date = QDate::currentDate().toString("dd/MM/yyyy");
    QRect footerRect(0, writer.height() - 50, writer.width(), 30);
    painter.setFont(QFont("Arial", 10));
    painter.setPen(Qt::black);
    painter.drawText(footerRect, Qt::AlignRight | Qt::AlignVCenter, "Généré le " + date);

    painter.end();
    QMessageBox::information(this, "PDF", "PDF généré avec succès !");
}
void MainWindow::on_statistique_clicked()
{
    statistique stats;
    stats.exec();
}
void MainWindow::on_lire_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row == -1) {
        QMessageBox::information(this, "Information", "Veuillez sélectionner une boutique à lire.");
        return;
    }

    QString nom = ui->tableWidget->item(row, 1)->text();
    QString statut = ui->tableWidget->item(row, 2)->text();
    QString localisation = ui->tableWidget->item(row, 3)->text();
    QString contact = ui->tableWidget->item(row, 4)->text();
    QString categorie = ui->tableWidget->item(row, 5)->text();

    QString texte = "Nom de la boutique : " + nom +
                    ". Statut : " + statut +
                    ". Localisation : " + localisation +
                    ". Contact : " + contact +
                    ". Catégorie : " + categorie + ".";

    // Créer un objet speech
    QTextToSpeech *speech = new QTextToSpeech(this);

    // Liste des locales disponibles par cet objet
    QList<QLocale> locales = speech->availableLocales();
    QStringList langues;
    for (const QLocale &locale : locales) {
        langues << QLocale::languageToString(locale.language());
    }

    bool ok;
    QString selectedLang = QInputDialog::getItem(this, "Choix de la langue", "Langue :", langues, 0, false, &ok);
    if (!ok || selectedLang.isEmpty()) return;

    // Trouver la locale choisie
    for (const QLocale &locale : locales) {
        if (QLocale::languageToString(locale.language()) == selectedLang) {
            speech->setLocale(locale);
            break;
        }
    }

    // Parler
    speech->say(texte);
}
void MainWindow::afficherCarteInteractive()
{
    QDialog *carteDialog = new QDialog(this);
    carteDialog->setWindowTitle("🗺️ Carte Interactive");
    carteDialog->resize(1000, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(carteDialog);

    // 🗣️ Synthèse vocale
    QTextToSpeech *speech = new QTextToSpeech(carteDialog);
    speech->say("Bienvenue dans la carte interactive du centre commercial.");

    // 🔎 Champ de recherche
    QLineEdit *recherche = new QLineEdit();
    recherche->setPlaceholderText("Rechercher une boutique...");
    mainLayout->addWidget(recherche);

    // 🧱 Grille des étages
    QGridLayout *etageLayout = new QGridLayout();

    QMap<QString, QLabel*> marqueurs; // 📍 Un marqueur par étage
    QMap<QString, QWidget*> etageWidgets; // Widgets des étages
    QStringList nomsEtages = {"1er Étage", "2ème Étage", "3ème Étage", "4ème Étage"};
    QStringList couleurs = {"#e3f2fd", "#e8f5e9", "#ffebee", "#ede7f6"};

    // Création des widgets d'étage
    for (int i = 0; i < nomsEtages.size(); ++i) {
        QWidget *etageWidget = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(etageWidget);
        QLabel *titre = new QLabel("<b>" + nomsEtages[i] + "</b>");
        titre->setAlignment(Qt::AlignCenter);

        QLabel *marker = new QLabel("📍");
        marker->setAlignment(Qt::AlignCenter);
        marker->setStyleSheet("font-size: 24px;");
        marker->hide();

        layout->addWidget(titre);
        layout->addStretch();
        layout->addWidget(marker);
        etageWidget->setStyleSheet(QString("background-color: %1; border-radius: 10px; padding: 10px;").arg(couleurs[i]));

        marqueurs[nomsEtages[i]] = marker;
        etageWidgets[nomsEtages[i]] = etageWidget;
        etageLayout->addWidget(etageWidget, 0, i);
    }

    mainLayout->addLayout(etageLayout);

    // 📌 Emplacement des boutiques (clé = nom boutique en minuscules)
    QMap<QString, QString> boutiques;
    boutiques.insert("bershka", "1er Étage");
    boutiques.insert("chnafer", "2ème Étage");
    boutiques.insert("pullandbear", "2ème Étage");
    boutiques.insert("zara", "2ème Étage");
    boutiques.insert("majesty mud", "3ème Étage");
    boutiques.insert("point m", "3ème Étage");
    boutiques.insert("barsha", "3ème Étage");
    boutiques.insert("kiabi", "3ème Étage");
    boutiques.insert("koton", "4ème Étage");
    boutiques.insert("fatale", "4ème Étage");
    boutiques.insert("adidas", "3ème Étage");
    boutiques.insert("dior", "3ème Étage");
    boutiques.insert("celio", "1er Étage");





    // 🎯 Recherche
    connect(recherche, &QLineEdit::textChanged, this, [=](const QString &text){
        QString lowerInput = text.trimmed().toLower();

        // Cacher tous les marqueurs
        for (auto marker : marqueurs) {
            marker->hide();
        }

        // Réinitialiser les styles des étages
        for (auto etage : etageWidgets) {
            etage->setStyleSheet(etage->styleSheet().replace(QRegularExpression("border: [^;]+;"), ""));
        }

        // Recherche exacte, insensible à la casse
        QMap<QString, QString>::const_iterator it = boutiques.constBegin();
        while (it != boutiques.constEnd()) {
            if (it.key().toLower() == lowerInput) {
                QString etage = it.value();

                // Montrer le marqueur
                marqueurs[etage]->show();

                // Ajouter un effet ping (bordure jaune)
                etageWidgets[etage]->setStyleSheet(
                    etageWidgets[etage]->styleSheet() +
                    "border: 3px solid yellow; animation: ping 1s;"
                    );

                // Annonce vocale
                speech->say("La boutique " + it.key() + " se trouve au " + etage);

                // Ajouter l'animation CSS
                carteDialog->setStyleSheet(
                    "@keyframes ping {"
                    "  0% { transform: scale(1); opacity: 1; }"
                    "  50% { transform: scale(1.05); opacity: 0.8; }"
                    "  100% { transform: scale(1); opacity: 1; }"
                    "}"
                    );
                break;
            }
            ++it;
        }
    });

    // 🔙 Bouton retour
    QPushButton *retour = new QPushButton("Retour");
    retour->setStyleSheet("background-color: #e91e63; color: white; padding: 6px 16px; border-radius: 5px;");
    connect(retour, &QPushButton::clicked, carteDialog, &QDialog::accept);
    mainLayout->addWidget(retour, 0, Qt::AlignRight);

    carteDialog->exec();
}


void MainWindow::readFromArduino()
{
    buffer += arduino->readAll();

    if (buffer.contains('\n')) {
        QString message = buffer.trimmed();
        qDebug() << "Message reçu :" << message;

        if (message.startsWith("SURTENSION")) {
            // Exemple de message reçu : SURTENSION;ID=3
            QStringList parts = message.split(';');
            if (parts.size() == 2 && parts[1].startsWith("ID=")) {
                QString idString = parts[1].section('=', 1, 1);
                int id = idString.toInt();
                if (id > 0) {
                    traiterSurtension(id);
                }
            }

            // Action facultative : envoyer confirmation à Arduino
            arduino->write("COUPER_ALIM\n");
        }

        buffer.clear(); // important de vider pour le prochain message
    }
}


void MainWindow::traiterSurtension(int id)
{
    QSqlQuery query;

    QString today = QDate::currentDate().toString("dd-MM-yyyy"); // Date format OK for your database

    // 1. Update the database (only backend, no UI table refresh)
    query.prepare("UPDATE RESSOURCES SET ETATR = 'En panne', DM = TO_DATE(:date, 'DD-MM-YYYY') WHERE IDR = :id");
    query.bindValue(":date", today);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✅ Ressource ID" << id << "mise à jour : En panne.";

        // 2. Fetch the resource NAME (NOMR)
        QSqlQuery getNameQuery;
        getNameQuery.prepare("SELECT NOMR FROM RESSOURCES WHERE IDR = :id");
        getNameQuery.bindValue(":id", id);

        QString nomRessource = "Inconnu";

        if (getNameQuery.exec() && getNameQuery.next()) {
            nomRessource = getNameQuery.value(0).toString();
        }

        // 3. Show QMessageBox
        QMessageBox::warning(this, "SURTENSION DÉTECTÉE",
                             "La ressource '" + nomRessource + "' est passée à l'état 'En panne' !");

        // ❌ Removed afficherRessources(); (no refresh table)

    } else {
        qDebug() << "❌ Erreur de mise à jour : " << query.lastError().text();
    }
}

