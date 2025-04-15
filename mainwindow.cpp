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




using qrcodegen::QrCode;
using qrcodegen::QrSegment;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox_Tri->addItems({
        "Tri par quantité (↑)",
        "Tri par quantité (↓)",
        "Tri par prix (↑)",
        "Tri par prix (↓)"
    });

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
    connect(ui->pushButton_tri, &QPushButton::clicked, this, &MainWindow::TrierParDisponibilite);
    connect(ui->calendar_maintenance, &QCalendarWidget::clicked, this, &MainWindow::afficherRessourcesPourDate);
    connect(ui->pushButton_Tri, &QPushButton::clicked, this, [=]() {
        QString mode = ui->comboBox_Tri->currentText();  // ou le vrai nom de ta comboBox

        if (mode == "Tri par quantité (↑)") {
            trierTableParColonne(3, Qt::AscendingOrder);  // colonne 3 = Quantité
        } else if (mode == "Tri par quantité (↓)") {
            trierTableParColonne(3, Qt::DescendingOrder);
        } else if (mode == "Tri par prix (↑)") {
            trierTableParColonne(4, Qt::AscendingOrder);  // colonne 4 = Prix
        } else if (mode == "Tri par prix (↓)") {
            trierTableParColonne(4, Qt::DescendingOrder);
        }
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
    if (ui->lineEdit_11->text().isEmpty() || ui->comboBox_type->currentText().isEmpty() ||
        ui->comboBox_etat->currentText().isEmpty() || ui->lineEdit_7->text().isEmpty() ||
        ui->lineEdit_8->text().isEmpty() || ui->lineEdit_9->text().isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int quantite = ui->lineEdit_7->text().toInt(&ok);
    if (!ok || quantite < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide (doit être un nombre positif).");
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

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO RESSOURCES (IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM)
        VALUES (RESSOURCES_SEQ.NEXTVAL, :nom, :type, 'Disponible', :quantite, :prix, :localisation,
                TO_DATE(:dateAchat, 'DD-MM-YYYY'), TO_DATE(:dateMaintenance, 'DD-MM-YYYY'))
    )");

    query.bindValue(":nom", ui->lineEdit_11->text());
    query.bindValue(":type", ui->comboBox_type->currentText());
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);
    query.bindValue(":localisation", ui->lineEdit_9->text());
    query.bindValue(":dateAchat", dateAchat.toString("dd-MM-yyyy"));
    query.bindValue(":dateMaintenance", dateMaintenance.toString("dd-MM-yyyy"));

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout : " + query.lastError().text());
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }

    // ✅ Génération automatique du code QR
    QString chemin = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                     + "/" + ui->lineEdit_11->text() + "_QRCode.png";
    genererQRCode(ui->lineEdit_11->text(), chemin);

    // ✅ Mise à jour interface
    QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès.");
    afficherRessources();
    on_comboBox_tri_currentTextChanged(ui->comboBox_tri->currentText());
    afficherDatesMaintenance();


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
    QString oldType = ui->tableWidget->item(selectedRow, 2)->text();
    QString oldEtat = ui->tableWidget->item(selectedRow, 3)->text();
    int oldQuantite = ui->tableWidget->item(selectedRow, 4)->text().toInt();
    float oldPrix = ui->tableWidget->item(selectedRow, 5)->text().toFloat();
    QString oldLocalisation = ui->tableWidget->item(selectedRow, 6)->text();
    QDate oldDateAchat = QDate::fromString(ui->tableWidget->item(selectedRow, 7)->text(), "dd-MM-yyyy");
    QDate oldDateMaintenance = QDate::fromString(ui->tableWidget->item(selectedRow, 8)->text(), "dd-MM-yyyy");

    // Nouvelles valeurs (ou anciennes si vide)
    QString nom = ui->lineEdit_11->text().trimmed();
    QString type = ui->comboBox_type->currentText();
    QString etat = ui->comboBox_etat->currentText();
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
    on_comboBox_tri_currentTextChanged(ui->comboBox_tri->currentText());

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

    QString nom = ui->tableWidget->item(selectedRow, 0)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cette ressource ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM RESSOURCES WHERE NOMR=:nom");
        query.bindValue(":nom", nom);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Ressource supprimée avec succès.");
            afficherRessources();
            afficherDatesMaintenance();  // ⬅️ ajoute cette ligne

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
        QString nom = ui->tableWidget->item(row, 0)->text();
        int quantite = ui->tableWidget->item(row, 3)->text().toInt();
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
    QString etat = ui->comboBox_tri->currentText();
    QSqlQuery query;

    if (etat == "Tout afficher") {
        query.prepare("SELECT NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM RESSOURCES");
    } else {
        query.prepare("SELECT NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM RESSOURCES WHERE ETATR = :etat");
        query.bindValue(":etat", etat);
    }

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'affichage : " + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 8; col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}
void MainWindow::on_comboBox_tri_currentTextChanged(const QString &etat)
{
    if (etat == "Disponible" || etat == "En panne" || etat == "En maintenance" || etat == "Réservé") {
        QSqlQuery query;
        query.prepare("SELECT NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM RESSOURCES WHERE ETATR = :etat");
        query.bindValue(":etat", etat);

        if (query.exec()) {
            ui->tableWidget->setRowCount(0);
            int row = 0;
            while (query.next()) {
                ui->tableWidget->insertRow(row);
                for (int col = 0; col < 8; ++col) {
                    ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
                }
                row++;
            }
        }
    } else {
        afficherRessources();  // Si on sélectionne une valeur vide ou par défaut, on affiche tout
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

    QString nomRessource = ui->tableWidget->item(selectedRow, 0)->text();  // Colonne 0 = Nom

    // Demander l'emplacement du fichier
    QString chemin = QFileDialog::getSaveFileName(this, "Enregistrer le QR Code", nomRessource + ".png", "Images PNG (*.png)");
    if (chemin.isEmpty())
        return;

    // Générer le QR code
    if (QrCodeGenerator::genererQRCode(nomRessource, chemin)) {
        QMessageBox::information(this, "Succès", "QR Code généré et enregistré avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la génération du QR Code.");
    }
}
void MainWindow::appliquerTri()
{
        QString mode = ui->comboBox_Tri->currentText();

        // Trouver l’index de la colonne à trier
        int colonneTri = -1;
        Qt::SortOrder ordre = Qt::AscendingOrder;

        if (mode == "Tri par quantité (↑)") {
            colonneTri = 3;  // Colonne "Quantité"
            ordre = Qt::AscendingOrder;
        } else if (mode == "Tri par quantité (↓)") {
            colonneTri = 3;
            ordre = Qt::DescendingOrder;
        } else if (mode == "Tri par prix (↑)") {
            colonneTri = 4;  // Colonne "Prix"
            ordre = Qt::AscendingOrder;
        } else if (mode == "Tri par prix (↓)") {
            colonneTri = 4;
            ordre = Qt::DescendingOrder;
        }

        if (colonneTri != -1) {
            ui->tableWidget->sortItems(colonneTri, ordre);
        }
}



void MainWindow::trierTableParColonne(int colonne, Qt::SortOrder ordre)
{
        ui->tableWidget->sortItems(colonne, ordre);
}
