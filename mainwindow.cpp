#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QLocale>
#include "qrcode.h"
#include "qrcodegen.hpp"
#include <QStandardPaths>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include "camembertdialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>



using qrcodegen::QrCode;
using qrcodegen::QrSegment;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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



    // Validation : Le nom et le type ne doivent contenir que des lettres et des espaces
    QRegularExpression regexNomType("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]+$");
    QRegularExpressionValidator *validatorNomType = new QRegularExpressionValidator(regexNomType, this);
    ui->lineEdit_11->setValidator(validatorNomType);

    // Validation : Quantité (nombre entier positif)
    QIntValidator *validatorQuantite = new QIntValidator(0, 99999, this);
    ui->lineEdit_7->setValidator(validatorQuantite);

    // Validation : Prix (nombre décimal positif)
    QDoubleValidator *validatorPrix = new QDoubleValidator(0, 99999, 2, this);
    validatorPrix->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_8->setValidator(validatorPrix);

    ui->comboBox_tri->clear();
    ui->comboBox_tri->addItems({"Tout afficher", "Disponible", "En panne", "En maintenance", "Réservé"});
    ui->comboBox_type->addItems({"Meuble", "Informatique", "Électronique","Climatisation"});
    ui->comboBox_etat->addItems({"Disponible", "En panne", "En maintenance", "Réservé"});

    connect(ui->pushButton_valider, &QPushButton::clicked, this, &MainWindow::ajouterRessource);
    connect(ui->pushButton_modif, &QPushButton::clicked, this, &MainWindow::modifierRessource);
    connect(ui->pushButton_supp, &QPushButton::clicked, this, &MainWindow::supprimerRessource);
    connect(ui->pushButton_stats, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesCamembert);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::exporterPDF);
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &MainWindow::TrierParDisponibilite);
    ui->comboBox_tri->clear();  // 🔁 Évite la duplication
    ui->comboBox_tri->addItems({
        "Tout afficher",
        "Disponible",
        "En panne",
        "En maintenance",
        "Réservé",
        "Tri par prix (↑)",
        "Tri par prix (↓)"
    });
    connect(ui->calendar_maintenance, &QCalendarWidget::clicked, this, &MainWindow::afficherRessourcesPourDate);
    connect(ui->lineEdit_recherche, &QLineEdit::textChanged, this, &MainWindow::filtrerRessources);
    connect(ui->pushButton_recherche, &QPushButton::clicked, this, [=]() {
        ui->lineEdit_recherche->clear();         // Vide le champ
        filtrerRessources();                     // Réaffiche tout
    });

    afficherRessources();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afficherRessources()
{
    QSqlQuery query("SELECT IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM RESSOURCES");

    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Type", "État", "Quantité", "Prix", "Localisation", "Date Achat", "Date Maintenance"});

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // ID non modifiable
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    ui->tableWidget->setColumnHidden(0, true);  // cacher la colonne ID
}


void MainWindow::ajouterRessource()
{
    // Vérifie que tous les champs nécessaires sont remplis
    if (ui->lineEdit_11->text().isEmpty() ||
        ui->comboBox_type->currentText().isEmpty() ||
        ui->comboBox_etat->currentText().isEmpty() ||
        ui->lineEdit_7->text().isEmpty() ||
        ui->lineEdit_8->text().isEmpty() ||
        ui->lineEdit_9->text().isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis.");
        return;
    }

    // Vérifie la validité des champs numériques
    bool ok;
    int quantite = ui->lineEdit_7->text().toInt(&ok);
    if (!ok || quantite < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide (doit être un entier positif).");
        return;
    }

    float prix = ui->lineEdit_8->text().toFloat(&ok);
    if (!ok || prix < 0) {
        QMessageBox::warning(this, "Erreur", "Prix invalide (doit être un nombre positif).");
        return;
    }

    QDate dateAchat = ui->dateEdit_3->date();
    QDate dateMaintenance = ui->dateEdit_4->date();

    if (dateMaintenance < dateAchat) {
        QMessageBox::warning(this, "Erreur", "La date de maintenance ne peut pas être antérieure à la date d'achat.");
        return;
    }

    // Préparation de la requête SQL d'insertion
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO RESSOURCES (IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM)
        VALUES (RESSOURCES_SEQ.NEXTVAL, :nom, :type, :etat, :quantite, :prix, :localisation,
                TO_DATE(:dateAchat, 'DD-MM-YYYY'), TO_DATE(:dateMaintenance, 'DD-MM-YYYY'))
    )");

    // Liaison des valeurs
    query.bindValue(":nom", ui->lineEdit_11->text().trimmed());
    query.bindValue(":type", ui->comboBox_type->currentText());
    query.bindValue(":etat", ui->comboBox_etat->currentText());
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);
    query.bindValue(":localisation", ui->lineEdit_9->text().trimmed());
    query.bindValue(":dateAchat", dateAchat.toString("dd-MM-yyyy"));
    query.bindValue(":dateMaintenance", dateMaintenance.toString("dd-MM-yyyy"));

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout : " + query.lastError().text());
        qDebug() << "Erreur SQL : " << query.lastError().text();
        return;
    }

    // ✅ Génération d’un QR code avec le nom
    QString chemin = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                     + "/" + ui->lineEdit_11->text().trimmed() + "_QRCode.png";
    genererQRCode(ui->lineEdit_11->text().trimmed(), chemin);

    QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès.");

    // Rafraîchissement de l’interface
    afficherRessources();
    afficherDatesMaintenance();
    TrierParDisponibilite();

    // Nettoyage des champs
    ui->lineEdit_11->clear();
    ui->comboBox_type->setCurrentIndex(-1);
    ui->comboBox_etat->setCurrentIndex(-1);
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->dateEdit_3->clear();
    ui->dateEdit_4->clear();
}



// ✅ 2. modifierRessource corrigée :
void MainWindow::modifierRessource()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ressource à modifier !");
        return;
    }

    // Récupérer l'ID directement depuis la colonne cachée
    int idR = ui->tableWidget->item(selectedRow, 0)->text().toInt();

    // Récupérer les anciennes valeurs
    QString oldNom = ui->tableWidget->item(selectedRow, 1)->text();
    QString oldType = ui->tableWidget->item(selectedRow, 2)->text();
    QString oldEtat = ui->tableWidget->item(selectedRow, 3)->text();
    int oldQuantite = ui->tableWidget->item(selectedRow, 4)->text().toInt();
    float oldPrix = ui->tableWidget->item(selectedRow, 5)->text().toFloat();
    QString oldLocalisation = ui->tableWidget->item(selectedRow, 6)->text();
    QDate oldDateAchat = QDate::fromString(ui->tableWidget->item(selectedRow, 7)->text(), "yyyy-MM-dd");
    QDate oldDateMaintenance = QDate::fromString(ui->tableWidget->item(selectedRow, 8)->text(), "yyyy-MM-dd");

    // Nouvelles valeurs (ou anciennes si vide)
    QString nom = ui->lineEdit_11->text().trimmed().isEmpty() ? oldNom : ui->lineEdit_11->text().trimmed();
    QString type = ui->comboBox_type->currentText().isEmpty() ? oldType : ui->comboBox_type->currentText();
    QString etat = ui->comboBox_etat->currentText().isEmpty() ? oldEtat : ui->comboBox_etat->currentText();
    int quantite = ui->lineEdit_7->text().trimmed().isEmpty() ? oldQuantite : ui->lineEdit_7->text().trimmed().toInt();
    float prix = ui->lineEdit_8->text().trimmed().isEmpty() ? oldPrix : ui->lineEdit_8->text().trimmed().toFloat();
    QString localisation = ui->lineEdit_9->text().trimmed().isEmpty() ? oldLocalisation : ui->lineEdit_9->text().trimmed();
    QDate dateAchat = ui->dateEdit_3->date().isValid() ? ui->dateEdit_3->date() : oldDateAchat;
    QDate dateMaintenance = ui->dateEdit_4->date().isValid() ? ui->dateEdit_4->date() : oldDateMaintenance;

    // Vérifications
    if (quantite < 0 || prix < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité et prix doivent être positifs.");
        return;
    }

    if (dateMaintenance < dateAchat) {
        QMessageBox::warning(this, "Erreur", "Date de maintenance ne peut pas être avant la date d'achat.");
        return;
    }

    // Mise à jour
    QSqlQuery query;
    query.prepare(R"(
        UPDATE RESSOURCES
        SET NOMR=:nom, TYPER=:type, ETATR=:etat, QUANTITER=:quantite, PRIXR=:prix, LOCR=:localisation,
            DAR=TO_DATE(:dateAchat, 'DD-MM-YYYY'), DM=TO_DATE(:dateMaintenance, 'DD-MM-YYYY')
        WHERE IDR=:id
    )");

    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);
    query.bindValue(":localisation", localisation);
    query.bindValue(":dateAchat", dateAchat.toString("dd-MM-yyyy"));
    query.bindValue(":dateMaintenance", dateMaintenance.toString("dd-MM-yyyy"));
    query.bindValue(":id", idR);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Échec de la modification : " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", "Ressource modifiée avec succès !");
    afficherRessources();
    afficherDatesMaintenance();
    TrierParDisponibilite();

    // Nettoyage
    ui->lineEdit_11->clear();
    ui->comboBox_type->setCurrentIndex(-1);
    ui->comboBox_etat->setCurrentIndex(-1);
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->dateEdit_3->clear();
    ui->dateEdit_4->clear();
}



void MainWindow::supprimerRessource()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ressource à supprimer !");
        return;
    }

    // ✅ On récupère l'ID de la ressource (colonne 0 cachée)
    int id = ui->tableWidget->item(selectedRow, 0)->text().toInt();

    // ✅ Pour affichage plus clair dans le message
    QString nom = ui->tableWidget->item(selectedRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Confirmation",
        "Voulez-vous vraiment supprimer la ressource '" + nom + "' ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM RESSOURCES WHERE IDR = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Ressource supprimée avec succès.");
            afficherRessources();
            afficherDatesMaintenance();
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression : " + query.lastError().text());
        }
    }
}
class CamembertWidget : public QWidget {
    QMap<QString, int> ressources;
public:
    CamembertWidget(QMap<QString, int> data, QWidget *parent = nullptr) : QWidget(parent), ressources(data) {
        setMinimumSize(400, 400);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRectF rect(50, 50, 300, 300);

        int total = 0;
        for (auto val : ressources.values()) total += val;

        QVector<QColor> couleurs = {
            QColor("#2f466c"), QColor("#6ea8cc"), QColor("#a3bcb6"),
            QColor("#88c0d0"), QColor("#b5c9ca")
        };

        float startAngle = 0.0;
        int i = 0;
        for (auto it = ressources.begin(); it != ressources.end(); ++it, ++i) {
            float spanAngle = 360.0 * it.value() / total;
            painter.setBrush(couleurs[i % couleurs.size()]);
            painter.setPen(Qt::NoPen);
            painter.drawPie(rect, int(startAngle * 16), int(spanAngle * 16));
            startAngle += spanAngle;
        }

        // Légende
        int y = 370;
        i = 0;
        for (auto it = ressources.begin(); it != ressources.end(); ++it, ++i) {
            painter.setBrush(couleurs[i % couleurs.size()]);
            painter.drawRect(50, y, 15, 15);
            painter.setPen(Qt::black);
            painter.drawText(70, y + 12, it.key() + " (" + QString::number(it.value()) + ")");
            y += 20;
        }
    }
};

void MainWindow::afficherStatistiquesCamembert()
{
    QMap<QString, int> ressources;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        // ✅ Colonne 1 = Nom | Colonne 4 = Quantité
        QString nom = ui->tableWidget->item(row, 1)->text();
        int quantite = ui->tableWidget->item(row, 4)->text().toInt();
        ressources[nom] += quantite;
    }

    if (ressources.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucune ressource à afficher.");
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Ressources les plus utilisées");
    dialog->resize(420, 450);
    dialog->setStyleSheet("background-color: #f5ede0; border: 2px solid #2f466c;");

    CamembertWidget *camembert = new CamembertWidget(ressources);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(camembert);
    dialog->setLayout(layout);
    dialog->exec();
}



void MainWindow::exporterPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "*.pdf");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setFullPage(true);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier PDF.");
        return;
    }

    QRectF pageRect = printer.pageRect(QPrinter::Point);
    painter.fillRect(pageRect, QColor("#f5ede0"));
    int pageWidth = pageRect.width();

    int y = 80;

    // 🎨 Couleurs
    QColor headerBgColor("#2f466c");
    QColor headerTextColor(Qt::white);
    QColor borderColor(Qt::black);

    // 🖋️ Cadre du titre
    int titleHeight = 40;
    int titleWidth = 300;
    int titleX = (pageWidth - titleWidth) / 2;
    QRect titleRect(titleX, y, titleWidth, titleHeight);
    painter.setBrush(headerBgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(titleRect, 10, 10);

    // ✏️ Texte du titre
    QFont titreFont("Arial", 12, QFont::Bold);
    painter.setFont(titreFont);
    painter.setPen(headerTextColor);
    painter.drawText(titleRect, Qt::AlignCenter, "📦 Liste des ressources");

    y += titleHeight + 20;

    // 📅 Date centrée
    QFont infoFont("Arial", 9);
    painter.setFont(infoFont);
    painter.setPen(Qt::black);
    QString dateHeure = QLocale(QLocale::French).toString(QDate::currentDate(), "dddd d MMMM yyyy")
                        + " à " + QTime::currentTime().toString("hh:mm");
    painter.drawText(QRect(0, y, pageWidth, 20), Qt::AlignCenter, dateHeure);
    y += 40;

    // 🗂️ Tableau compact
    int rowHeight = 22;
    int columnWidth = 70;
    int startX = (pageWidth - (ui->tableWidget->columnCount() * columnWidth)) / 2;

    QFont cellFont("Arial", 8);
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    painter.setPen(Qt::NoPen);

    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        QRect rect(startX + col * columnWidth, y, columnWidth, rowHeight);
        painter.setBrush(headerBgColor);
        painter.drawRoundedRect(rect, 5, 5);
        painter.setPen(headerTextColor);
        painter.drawText(rect, Qt::AlignCenter, ui->tableWidget->horizontalHeaderItem(col)->text());
        painter.setPen(Qt::NoPen);
    }

    y += rowHeight;

    // 📄 Données
    painter.setFont(cellFont);
    painter.setPen(borderColor);
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QRect rect(startX + col * columnWidth, y, columnWidth, rowHeight);
            painter.drawRect(rect);

            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString text = item ? item->text() : "";
            painter.drawText(rect.adjusted(2, 0, -2, 0), Qt::AlignCenter, text);
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "PDF exporté avec succès !");
}

void MainWindow::TrierParDisponibilite()
{
    QString choix = ui->comboBox_tri->currentText();
    QString conditionEtat;
    QString tri;

    // 1. Gestion de la condition d'état
    if (choix == "Disponible" || choix == "En panne" || choix == "En maintenance" || choix == "Réservé") {
        conditionEtat = " WHERE ETATR = :etat";
    }

    // 2. Gestion du tri
    if (choix == "Tri par prix (↑)") {
        tri = " ORDER BY PRIXR ASC";
    } else if (choix == "Tri par prix (↓)") {
        tri = " ORDER BY PRIXR DESC";
    }

    // 3. Cas combiné : état + tri par prix
    QString etatFiltre = "";
    if (conditionEtat.contains(":etat")) {
        etatFiltre = choix;  // le texte correspond directement à l’état
    }

    // 4. Requête SQL
    QString queryStr = "SELECT IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM RESSOURCES" + conditionEtat + tri;
    QSqlQuery query;
    query.prepare(queryStr);
    if (!etatFiltre.isEmpty()) query.bindValue(":etat", etatFiltre);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération : " + query.lastError().text());
        return;
    }

    // 5. Affichage dans le tableau
    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // ID non modifiable
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    ui->tableWidget->setColumnHidden(0, true);  // Cacher colonne ID
}

void MainWindow::filtrerRessources()
{
    QString texteRecherche = ui->lineEdit_recherche->text().trimmed();

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        bool afficher = false;

        for (int j = 0; j < ui->tableWidget->columnCount(); ++j) {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            if (item && item->text().contains(texteRecherche, Qt::CaseInsensitive)) {
                afficher = true;
                break;
            }
        }

        ui->tableWidget->setRowHidden(i, !afficher);
    }
}


void MainWindow::genererQRCode(const QString &texte, const QString &nomFichier) {
    using qrcodegen::QrCode;
    using qrcodegen::QrSegment;

    const QrCode qr = QrCode::encodeText(texte.toUtf8().constData(), QrCode::Ecc::LOW);
    const int taille = qr.getSize();
    const int echelle = 10;
    const int imageSize = taille * echelle;

    QImage image(imageSize, imageSize, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setBrush(Qt::black);
    for (int y = 0; y < taille; ++y) {
        for (int x = 0; x < taille; ++x) {
            if (qr.getModule(x, y)) {
                painter.drawRect(x * echelle, y * echelle, echelle, echelle);
            }
        }
    }
    painter.end();

    image.save(nomFichier);
}
void MainWindow::afficherDatesMaintenance()
{
    ui->calendar_maintenance->setSelectedDate(QDate::currentDate()); // optionnel

    // On vide les anciennes mises en forme
    QMap<QDate, QTextCharFormat> formats = ui->calendar_maintenance->dateTextFormat();
    for (auto date : formats.keys()) {
        ui->calendar_maintenance->setDateTextFormat(date, QTextCharFormat());
    }

    // On crée le style
    QTextCharFormat format;
    format.setBackground(Qt::blue); // ou QColor("#8cb4ff") si tu veux une autre couleur
    format.setForeground(Qt::white);
    format.setFontWeight(QFont::Bold);

    QSqlQuery query("SELECT DISTINCT DM FROM RESSOURCES");

    while (query.next()) {
        QDate date = query.value(0).toDate();
        if (date.isValid()) {
            ui->calendar_maintenance->setDateTextFormat(date, format);
        }
    }
}
void MainWindow::afficherRessourcesPourDate(const QDate &date)
{
    QSqlQuery query;
    query.prepare("SELECT NOMR, TYPER, LOCR FROM RESSOURCES WHERE DM = TO_DATE(:date, 'YYYY-MM-DD')");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération : " + query.lastError().text());
        return;
    }

    QString info;
    while (query.next()) {
        QString nom = query.value(0).toString();
        QString type = query.value(1).toString();
        QString loc = query.value(2).toString();
        info += "🔧 " + nom + " (" + type + ") - " + loc + "\n";
    }

    if (info.isEmpty())
        info = "Aucune ressource à maintenir à cette date.";

    QMessageBox::information(this, "Maintenance du " + date.toString("dddd d MMMM yyyy"), info);
}
void MainWindow::on_pushButton_qr_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une ressource dans le tableau.");
        return;
    }

    // Récupération des données
    QString nom = ui->tableWidget->item(selectedRow, 1)->text();
    QString type = ui->tableWidget->item(selectedRow, 2)->text();
    QString etat = ui->tableWidget->item(selectedRow, 3)->text();
    QString quantite = ui->tableWidget->item(selectedRow, 4)->text();
    QString prix = ui->tableWidget->item(selectedRow, 5)->text();
    QString localisation = ui->tableWidget->item(selectedRow, 6)->text();
    QString dateAchat = ui->tableWidget->item(selectedRow, 7)->text();
    QString dateMaintenance = ui->tableWidget->item(selectedRow, 8)->text();

    // Contenu du QR Code
    QString contenuQR =
        "Nom : " + nom + "\n" +
        "Type : " + type + "\n" +
        "État : " + etat + "\n" +
        "Quantité : " + quantite + "\n" +
        "Prix : " + prix + "\n" +
        "Localisation : " + localisation + "\n" +
        "Date d'achat : " + dateAchat + "\n" +
        "Date maintenance : " + dateMaintenance;

    // Génération du QR Code
    const QrCode qr = QrCode::encodeText(contenuQR.toUtf8().constData(), QrCode::Ecc::LOW);
    const int taille = qr.getSize();
    const int echelle = 10;
    const int imageSize = taille * echelle;

    QImage image(imageSize, imageSize, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setBrush(Qt::black);
    for (int y = 0; y < taille; ++y) {
        for (int x = 0; x < taille; ++x) {
            if (qr.getModule(x, y)) {
                painter.drawRect(x * echelle, y * echelle, echelle, echelle);
            }
        }
    }
    painter.end();

    // Affichage dans QMessageBox
    QPixmap qrPixmap = QPixmap::fromImage(image);
    QLabel* label = new QLabel;
    label->setPixmap(qrPixmap.scaled(250, 250, Qt::KeepAspectRatio));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("QR Code - Informations Ressource");
    dialog->setLayout(layout);
    dialog->exec();
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

    QString today = QDate::currentDate().toString("dd-MM-yyyy"); // Date du jour

    query.prepare("UPDATE RESSOURCES SET ETATR = 'En panne', DM = TO_DATE(:date, 'DD-MM-YYYY') WHERE IDR = :id");
    query.bindValue(":date", today);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✅ Ressource ID" << id << "mise à jour : En panne.";

        afficherRessources(); // Refresh du tableau

        // 1. On récupère le NOM de la ressource
        QSqlQuery getNameQuery;
        getNameQuery.prepare("SELECT NOMR FROM RESSOURCES WHERE IDR = :id");
        getNameQuery.bindValue(":id", id);

        QString nomRessource = "Inconnu";

        if (getNameQuery.exec() && getNameQuery.next()) {
            nomRessource = getNameQuery.value(0).toString();
        }

        // 2. Envoyer à Arduino
        QString message = "NOM:" + nomRessource + "\n"; // ➔ Ex : NOM:Ordinateur\n
        arduino->write(message.toUtf8()); // on envoie en format texte UTF8

        // 3. Popup Qt
        QMessageBox::warning(this, "SURTENSION DÉTECTÉE",
                             "La ressource '" + nomRessource + "' est passée à l'état 'En panne' !");
    } else {
        qDebug() << "❌ Erreur de mise à jour : " << query.lastError().text();
    }
}




