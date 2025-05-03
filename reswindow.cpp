#include "reswindow.h"
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
#include "qrcodegen.hpp"
#include <QStandardPaths>
#include <QMap>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

ResWindow::ResWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arduino = new QSerialPort(this);

    // Dynamic Arduino port detection
    QString arduinoPort;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        if (info.description().contains("Arduino", Qt::CaseInsensitive)) {
            arduinoPort = info.portName();
            break;
        }
    }
    if (arduinoPort.isEmpty()) {
        ui->label->setText("Aucun Arduino détecté");
    } else {
        arduino->setPortName(arduinoPort);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        if (arduino->open(QIODevice::ReadWrite)) {
            connect(arduino, &QSerialPort::readyRead, this, &ResWindow::readFromArduino);
        } else {
            ui->label->setText("Échec de connexion Arduino sur " + arduinoPort);
        }
    }

    // Validation: Name and type (letters and spaces)
    QRegularExpression regexNomType("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]+$");
    QRegularExpressionValidator *validatorNomType = new QRegularExpressionValidator(regexNomType, this);
    ui->lineEdit_11->setValidator(validatorNomType);

    // Validation: Quantity (positive integer)
    QIntValidator *validatorQuantite = new QIntValidator(0, 99999, this);
    ui->lineEdit_7->setValidator(validatorQuantite);

    // Validation: Price (positive decimal)
    QDoubleValidator *validatorPrix = new QDoubleValidator(0, 99999, 2, this);
    validatorPrix->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_8->setValidator(validatorPrix);

    // Initialize combo boxes
    ui->comboBox_type->addItems({"Meuble", "Informatique", "Électronique", "Climatisation"});
    ui->comboBox_etat->addItems({"Disponible", "En panne", "En maintenance", "Réservé"});
    ui->comboBox_tri->addItems({
        "Tout afficher",
        "Disponible",
        "En panne",
        "En maintenance",
        "Réservé",
        "Tri par prix (↑)",
        "Tri par prix (↓)"
    });

    // Connect signals
    connect(ui->pushButton_valider_3, &QPushButton::clicked, this, &ResWindow::ajouterRessource);
    connect(ui->pushButton_modif, &QPushButton::clicked, this, &ResWindow::modifierRessource);
    connect(ui->pushButton_supp, &QPushButton::clicked, this, &ResWindow::supprimerRessource);
    connect(ui->pushButton_stats, &QPushButton::clicked, this, &ResWindow::afficherStatistiquesCamembert);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &ResWindow::exporterPDF);
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &ResWindow::TrierParDisponibilite);
    connect(ui->calendar_maintenance, &QCalendarWidget::clicked, this, &ResWindow::afficherRessourcesPourDate);
    connect(ui->lineEdit_recherche, &QLineEdit::textChanged, this, &ResWindow::filtrerRessources);
    connect(ui->pushButton_recherche, &QPushButton::clicked, this, [=]() {
        ui->lineEdit_recherche->clear();
        filtrerRessources();
    });

    afficherRessources();
}

ResWindow::~ResWindow()
{
    delete ui;
}

void ResWindow::afficherRessources()
{
    QSqlQuery query("SELECT IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM IMENE.RESSOURCES");

    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Type", "État", "Quantité", "Prix", "Localisation", "Date Achat", "Date Maintenance"});

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    ui->tableWidget->setColumnHidden(0, true);
}

void ResWindow::ajouterRessource()
{
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

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO IMENE.RESSOURCES (IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM)
        VALUES (RESSOURCES_SEQ.NEXTVAL, :nom, :type, :etat, :quantite, :prix, :localisation,
                TO_DATE(:dateAchat, 'DD-MM-YYYY'), TO_DATE(:dateMaintenance, 'DD-MM-YYYY'))
    )");

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

    QString chemin = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                     + "/" + ui->lineEdit_11->text().trimmed() + "_QRCode.png";
    genererQRCode(ui->lineEdit_11->text().trimmed(), chemin);

    QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès.");

    afficherRessources();
    afficherDatesMaintenance();
    TrierParDisponibilite();

    ui->lineEdit_11->clear();
    ui->comboBox_type->setCurrentIndex(-1);
    ui->comboBox_etat->setCurrentIndex(-1);
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->dateEdit_3->clear();
    ui->dateEdit_4->clear();
}

void ResWindow::modifierRessource()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ressource à modifier !");
        return;
    }

    int idR = ui->tableWidget->item(selectedRow, 0)->text().toInt();

    QString oldNom = ui->tableWidget->item(selectedRow, 1)->text();
    QString oldType = ui->tableWidget->item(selectedRow, 2)->text();
    QString oldEtat = ui->tableWidget->item(selectedRow, 3)->text();
    int oldQuantite = ui->tableWidget->item(selectedRow, 4)->text().toInt();
    float oldPrix = ui->tableWidget->item(selectedRow, 5)->text().toFloat();
    QString oldLocalisation = ui->tableWidget->item(selectedRow, 6)->text();
    QDate oldDateAchat = QDate::fromString(ui->tableWidget->item(selectedRow, 7)->text(), "yyyy-MM-dd");
    QDate oldDateMaintenance = QDate::fromString(ui->tableWidget->item(selectedRow, 8)->text(), "yyyy-MM-dd");

    QString nom = ui->lineEdit_11->text().trimmed().isEmpty() ? oldNom : ui->lineEdit_11->text().trimmed();
    QString type = ui->comboBox_type->currentText().isEmpty() ? oldType : ui->comboBox_type->currentText();
    QString etat = ui->comboBox_etat->currentText().isEmpty() ? oldEtat : ui->comboBox_etat->currentText();
    int quantite = ui->lineEdit_7->text().trimmed().isEmpty() ? oldQuantite : ui->lineEdit_7->text().trimmed().toInt();
    float prix = ui->lineEdit_8->text().trimmed().isEmpty() ? oldPrix : ui->lineEdit_8->text().trimmed().toFloat();
    QString localisation = ui->lineEdit_9->text().trimmed().isEmpty() ? oldLocalisation : ui->lineEdit_9->text().trimmed();
    QDate dateAchat = ui->dateEdit_3->date().isValid() ? ui->dateEdit_3->date() : oldDateAchat;
    QDate dateMaintenance = ui->dateEdit_4->date().isValid() ? ui->dateEdit_4->date() : oldDateMaintenance;

    if (quantite < 0 || prix < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité et prix doivent être positifs.");
        return;
    }

    if (dateMaintenance < dateAchat) {
        QMessageBox::warning(this, "Erreur", "Date de maintenance ne peut pas être avant la date d'achat.");
        return;
    }

    QSqlQuery query;
    query.prepare(R"(
        UPDATE IMENE.RESSOURCES
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

    ui->lineEdit_11->clear();
    ui->comboBox_type->setCurrentIndex(-1);
    ui->comboBox_etat->setCurrentIndex(-1);
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->dateEdit_3->clear();
    ui->dateEdit_4->clear();
}

void ResWindow::supprimerRessource()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ressource à supprimer !");
        return;
    }

    int id = ui->tableWidget->item(selectedRow, 0)->text().toInt();
    QString nom = ui->tableWidget->item(selectedRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Confirmation",
        "Voulez-vous vraiment supprimer la ressource '" + nom + "' ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM IMENE.RESSOURCES WHERE IDR = :id");
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

void ResWindow::afficherStatistiquesCamembert()
{
    QMap<QString, int> ressources;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QString nom = ui->tableWidget->item(row, 1)->text();
        int quantite = ui->tableWidget->item(row, 4)->text().toInt();
        ressources[nom] += quantite;
    }

    if (ressources.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucune ressource à afficher.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Ressources les plus utilisées");
    dialog.resize(420, 450);
    dialog.setStyleSheet("background-color: #f5ede0; border: 2px solid #2f466c;");

    CamembertWidget *camembert = new CamembertWidget(ressources, &dialog);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(camembert);
    dialog.setLayout(layout);
    dialog.exec();
}

void ResWindow::exporterPDF()
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
    QColor headerBgColor("#2f466c");
    QColor headerTextColor(Qt::white);
    QColor borderColor(Qt::black);

    int titleHeight = 40;
    int titleWidth = 300;
    int titleX = (pageWidth - titleWidth) / 2;
    QRect titleRect(titleX, y, titleWidth, titleHeight);
    painter.setBrush(headerBgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(titleRect, 10, 10);

    QFont titreFont("Arial", 12, QFont::Bold);
    painter.setFont(titreFont);
    painter.setPen(headerTextColor);
    painter.drawText(titleRect, Qt::AlignCenter, "📦 Liste des ressources");

    y += titleHeight + 20;

    QFont infoFont("Arial", 9);
    painter.setFont(infoFont);
    painter.setPen(Qt::black);
    QString dateHeure = QLocale(QLocale::French).toString(QDate::currentDate(), "dddd d MMMM yyyy")
                        + " à " + QTime::currentTime().toString("hh:mm");
    painter.drawText(QRect(0, y, pageWidth, 20), Qt::AlignCenter, dateHeure);
    y += 40;

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

void ResWindow::TrierParDisponibilite()
{
    QString choix = ui->comboBox_tri->currentText();
    QString conditionEtat;
    QString tri;

    if (choix == "Disponible" || choix == "En panne" || choix == "En maintenance" || choix == "Réservé") {
        conditionEtat = " WHERE ETATR = :etat";
    }

    if (choix == "Tri par prix (↑)") {
        tri = " ORDER BY PRIXR ASC";
    } else if (choix == "Tri par prix (↓)") {
        tri = " ORDER BY PRIXR DESC";
    }

    QString etatFiltre = conditionEtat.contains(":etat") ? choix : "";
    QString queryStr = "SELECT IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM IMENE.RESSOURCES" + conditionEtat + tri;
    QSqlQuery query;
    query.prepare(queryStr);
    if (!etatFiltre.isEmpty()) query.bindValue(":etat", etatFiltre);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération : " + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    ui->tableWidget->setColumnHidden(0, true);
}

void ResWindow::filtrerRessources()
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

void ResWindow::genererQRCode(const QString &texte, const QString &nomFichier)
{
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

void ResWindow::afficherDatesMaintenance()
{
    ui->calendar_maintenance->setSelectedDate(QDate::currentDate());

    QMap<QDate, QTextCharFormat> formats = ui->calendar_maintenance ->dateTextFormat();
    for (auto date : formats.keys()) {
        ui->calendar_maintenance->setDateTextFormat(date, QTextCharFormat());
    }

    QTextCharFormat format;
    format.setBackground(Qt::blue);
    format.setForeground(Qt::white);
    format.setFontWeight(QFont::Bold);

    QSqlQuery query("SELECT DISTINCT DM FROM IMENE.RESSOURCES");

    while (query.next()) {
        QDate date = query.value(0).toDate();
        if (date.isValid()) {
            ui->calendar_maintenance->setDateTextFormat(date, format);
        }
    }
}

void ResWindow::afficherRessourcesPourDate(const QDate &date)
{
    QSqlQuery query;
    query.prepare("SELECT NOMR, TYPER, LOCR FROM IMENE.RESSOURCES WHERE DM = TO_DATE(:date, 'YYYY-MM-DD')");
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

void ResWindow::on_pushButton_qr_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une ressource dans le tableau.");
        return;
    }

    QString nom = ui->tableWidget->item(selectedRow, 1)->text();
    QString type = ui->tableWidget->item(selectedRow, 2)->text();
    QString etat = ui->tableWidget->item(selectedRow, 3)->text();
    QString quantite = ui->tableWidget->item(selectedRow, 4)->text();
    QString prix = ui->tableWidget->item(selectedRow, 5)->text();
    QString localisation = ui->tableWidget->item(selectedRow, 6)->text();
    QString dateAchat = ui->tableWidget->item(selectedRow, 7)->text();
    QString dateMaintenance = ui->tableWidget->item(selectedRow, 8)->text();

    QString contenuQR =
        "Nom : " + nom + "\n" +
        "Type : " + type + "\n" +
        "État : " + etat + "\n" +
        "Quantité : " + quantite + "\n" +
        "Prix : " + prix + "\n" +
        "Localisation : " + localisation + "\n" +
        "Date d'achat : " + dateAchat + "\n" +
        "Date maintenance : " + dateMaintenance;

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

    QPixmap qrPixmap = QPixmap::fromImage(image);
    QLabel* label = new QLabel;
    label->setPixmap(qrPixmap.scaled(250, 250, Qt::KeepAspectRatio));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    QDialog dialog(this);
    dialog.setWindowTitle("QR Code - Informations Ressource");
    dialog.setLayout(layout);
    dialog.exec();
}

void ResWindow::readFromArduino()
{
    buffer += arduino->readAll();

    if (buffer.contains('\n')) {
        QString message = buffer.trimmed();
        qDebug() << "Message reçu :" << message;

        if (message.startsWith("SURTENSION")) {
            QStringList parts = message.split(';');
            if (parts.size() == 2 && parts[1].startsWith("ID=")) {
                QString idString = parts[1].section('=', 1, 1);
                int id = idString.toInt();
                if (id > 0) {
                    traiterSurtension(id);
                }
            }
            arduino->write("COUPER_ALIM\n");
        }

        buffer.clear();
    }
}

void ResWindow::traiterSurtension(int id)
{
    QSqlQuery query;

    QString today = QDate::currentDate().toString("dd-MM-yyyy");

    query.prepare("UPDATE IMENE.RESSOURCES SET ETATR = 'En panne', DM = TO_DATE(:date, 'DD-MM-YYYY') WHERE IDR = :id");
    query.bindValue(":date", today);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✅ Ressource ID" << id << "mise à jour : En panne.";
        afficherRessources();

        QSqlQuery getNameQuery;
        getNameQuery.prepare("SELECT NOMR FROM IMENE.RESSOURCES WHERE IDR = :id");
        getNameQuery.bindValue(":id", id);

        QString nomRessource = "Inconnu";

        if (getNameQuery.exec() && getNameQuery.next()) {
            nomRessource = getNameQuery.value(0).toString();
        }

        QString message = "NOM:" + nomRessource + "\n";
        arduino->write(message.toUtf8());

        QMessageBox::warning(this, "SURTENSION DÉTECTÉE",
                             "La ressource '" + nomRessource + "' est passée à l'état 'En panne' !");
    } else {
        qDebug() << "❌ Erreur de mise à jour : " << query.lastError().text();
    }
}
