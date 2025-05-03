#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "evenements.h"
#include "HistoryDialog.h"
#include "camembertdialog.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QPrinter>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QLocale>

#include <QStandardPaths>
#include <QMap>
#include <QIcon>
#include <QSize>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include "qrcode.h"
#include "qrcodegen.hpp"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---------- EVENEMENT TABLE SETUP ----------
    evenement e;
    QSqlQueryModel* model = e.afficher();
    if (model) {
        ui->tableView4_3->setModel(model);
        ui->tableView4_3->setSortingEnabled(true);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec du chargement des données initiales");
    }

    connect(ui->comboBox_4, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onComboBox2IndexChanged);
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);

    ui->pushButtonNotification_3->setIcon(QIcon(":/images/cloche.png"));
    ui->pushButtonNotification_3->setIconSize(QSize(24, 24));
    ui->pushButtonNotification_3->setFlat(true);
    connect(ui->pushButtonNotification_3, &QPushButton::clicked, this, [this]() {
        checkUpcomingEvents();
    });

    // ---------- ARDUINO SETUP ----------
    arduino = new QSerialPort(this);
    arduino->setPortName("COM3"); // Adapt as needed
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    if
        (arduino->open(QIODevice::ReadWrite)) {
        connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readFromArduino);
    } else {
        ui->label_3->setText("Échec de connexion Arduino");
    }

    // ---------- VALIDATION SETUP ----------
    QRegularExpression regexNomType("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]+$");
    QRegularExpressionValidator *validatorNomType = new QRegularExpressionValidator(regexNomType, this);
    ui->lineEdit_nomres->setValidator(validatorNomType);

    QIntValidator *validatorQuantite = new QIntValidator(0, 99999, this);
    ui->lineEdit_quantres->setValidator(validatorQuantite);

    QDoubleValidator *validatorPrix = new QDoubleValidator(0, 99999, 2, this);
    validatorPrix->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_prixres->setValidator(validatorPrix);

    // ---------- COMBOBOX SETUP ----------
    ui->comboBox_tri_3->clear();
    ui->comboBox_tri_3->addItems({
        "Tout afficher",
        "Disponible",
        "En panne",
        "En maintenance",
        "Réservé",
        "Tri par prix (↑)",
        "Tri par prix (↓)"
    });

    ui->comboBox_type_3->addItems({
        "Meuble",
        "Informatique",
        "Électronique",
        "Climatisation"
    });

    ui->comboBox_etat_3->addItems({
        "Disponible",
        "En panne",
        "En maintenance",
        "Réservé"
    });

    // ---------- CONNECTIONS ----------
    connect(ui->pushButton_valider_3, &QPushButton::clicked, this, &MainWindow::ajouterRessource);
    connect(ui->pushButton_modif_3, &QPushButton::clicked, this, &MainWindow::modifierRessource);
    connect(ui->pushButton_supp_3, &QPushButton::clicked, this, &MainWindow::supprimerRessource);
    connect(ui->pushButton_stats_6, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesCamembert);
    connect(ui->pushButton_pdf_3, &QPushButton::clicked, this, &MainWindow::exporterPDF);
    connect(ui->comboBox_tri_3, &QComboBox::currentTextChanged, this, &MainWindow::TrierParDisponibilite);

    connect(ui->calendar_maintenance_3, &QCalendarWidget::clicked, this, &MainWindow::afficherRessourcesPourDate);
    connect(ui->lineEdit_recherche_3, &QLineEdit::textChanged, this, &MainWindow::filtrerRessources);
    connect(ui->pushButton_recherche_3, &QPushButton::clicked, this, [=]() {
        ui->lineEdit_recherche_3->clear();
        filtrerRessources();
    });

    afficherRessources();
}

MainWindow::~MainWindow() {
    delete ui;
}

// Definition of the onRowSelected slot
void MainWindow::onRowSelected()
{
    QModelIndexList selectedRows = ui->tableView4_3->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;

    QModelIndex index = selectedRows.first();

    QString nom = ui->tableView4_3->model()->data(index.sibling(index.row(), 1)).toString();
    QDate dateDebut = ui->tableView4_3->model()->data(index.sibling(index.row(), 2)).toDate();
    QDate dateFin = ui->tableView4_3->model()->data(index.sibling(index.row(), 3)).toDate();
    QString lieu = ui->tableView4_3->model()->data(index.sibling(index.row(), 4)).toString();
    int capacite = ui->tableView4_3->model()->data(index.sibling(index.row(), 5)).toInt();
    QString sponsors = ui->tableView4_3->model()->data(index.sibling(index.row(), 6)).toString();
    float budget = ui->tableView4_3->model()->data(index.sibling(index.row(), 7)).toFloat();

    // Remplir les champs du formulaire
    ui->lineedit_nom_3->setText(nom);
    ui->lineedit_date_debut_3->setDate(dateDebut);
    ui->lineedit_date_fin_3->setDate(dateFin);
    ui->comboBox_lieu_3->setCurrentText(lieu);
    ui->lineedit_capacite_3->setText(QString::number(capacite));
    ui->comboBox_sponsors_3->setCurrentText(sponsors);
    ui->lineedit_budget_3->setText(QString::number(budget));

    // Facultatif : Préremplir le champ de suppression avec le nom
    ui->lineedit_old_nom_3->setText(nom);
}

void MainWindow::on_pushbutton_ajouter_clicked()
{
    QString eventName = ui->lineedit_nom_3->text();
    QDate startDate = ui->lineedit_date_debut_3->date();
    QDate endDate = ui->lineedit_date_fin_3->date();
    float budget = ui->lineedit_budget_3->text().toFloat();
    int capacity = ui->lineedit_capacite_3->text().toInt();
    QString venue = ui->comboBox_lieu_3->currentText();
    QString sponsors = ui->comboBox_sponsors_3->currentText();

    // Input validation
    if (eventName.isEmpty() || venue.isEmpty() || sponsors.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    if (!venue.contains("bloc", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le lieu doit inclure 'bloc' (ex: 'bloc A').");
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

    if (ui->lineedit_date_debut_3->date()>ui->lineedit_date_fin_3->date()) {  // Fixed date comparison logic
        QMessageBox::warning(this, "Erreur", "La date de début doit être antérieure ou égale à la date de fin.");
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

    bool test = e.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Événement ajouté avec succès.");
        ui->tableView4_3->setModel(e.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'événement.");
    }
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);

}

void MainWindow::on_pushButton_supprimer_3_clicked()
{
    QString eventName = ui->lineedit_old_nom_3->text();

    if (eventName.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez spécifier le nom de l'événement à supprimer.");
        return;
    }

    evenement e;
    bool test = e.supprimer(eventName);
    if (test) {
        QMessageBox::information(this, "Succès", "Événement supprimé avec succès.");
        ui->tableView4_3->setModel(e.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'événement.");
    }
}

void MainWindow::on_pushButton_modifier_3_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView4_3->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à modifier.");
        return;
    }

    QString oldNom = ui->tableView4_3->model()->data(selectedIndexes.first().sibling(selectedIndexes.first().row(), 1)).toString();

    QString eventName = ui->lineedit_nom_3->text();
    QDate startDate = ui->lineedit_date_debut_3->date();
    QDate endDate = ui->lineedit_date_fin_3->date();
    float budget = ui->lineedit_budget_3->text().toFloat();
    int capacity = ui->lineedit_capacite_3->text().toInt();
    QString venue = ui->comboBox_lieu_3->currentText();
    QString sponsors = ui->comboBox_sponsors_3->currentText();

    // Input validation (same as ajouter)
    if (eventName.isEmpty() || venue.isEmpty() || sponsors.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    if (!venue.contains("bloc", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le lieu doit inclure 'bloc' (ex: 'bloc A').");
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

    if (startDate > endDate) {
        QMessageBox::warning(this, "Erreur", "La date de début doit être antérieure ou égale à la date de fin.");
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

    bool test = e.modifier(oldNom);
    if (test) {
        QMessageBox::information(this, "Succès", "Événement modifié avec succès.");
        ui->tableView4_3->setModel(e.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'événement.");
    }
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);

}

// Moved to MainWindow class instead of evenement
void MainWindow::on_pushButton_stats_3_clicked()  // Add this slot in header too
{
    evenement e;
    e.afficherStatistiques(this);
}

void MainWindow::on_pushButton_export_pdf_3_clicked()
{
    // Get selected row
    QModelIndexList selectedIndexes = ui->tableView4_3->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à exporter.");
        return;
    }

    // Get data from selected row with QDateTime to include time
    QModelIndex index = selectedIndexes.first();
    QString eventName = ui->tableView4_3->model()->data(index.sibling(index.row(), 1)).toString();
    QDateTime startDateTime = ui->tableView4_3->model()->data(index.sibling(index.row(), 2)).toDateTime();  // Include time
    QDateTime endDateTime = ui->tableView4_3->model()->data(index.sibling(index.row(), 3)).toDateTime();    // Include time
    QString venue = ui->tableView4_3->model()->data(index.sibling(index.row(), 4)).toString();
    int capacity = ui->tableView4_3->model()->data(index.sibling(index.row(), 5)).toInt();
    QString sponsors = ui->tableView4_3->model()->data(index.sibling(index.row(), 6)).toString();
    float budget = ui->tableView4_3->model()->data(index.sibling(index.row(), 7)).toFloat();

    // Open file dialog for PDF save location
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter l'événement en PDF",
                                                    QDir::homePath() + "/" + eventName + "_event.pdf",
                                                    "PDF Files (*.pdf)");
    if (fileName.isEmpty())
        return;

    // Set up PDF printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setFullPage(false);

    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(60, 60, 60, 60));
    printer.setPageLayout(layout);

    // Start painting on PDF
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Erreur", "Échec de l'ouverture du fichier PDF pour l'écriture");
        return;
    }

    // Set up fonts with enhanced styling
    QFont nameFont("Montserrat", 48, QFont::Bold);  // Larger and bolder for emphasis
    QFont textFont("Montserrat", 18);              // Slightly larger for readability
    QFont footerFont("Montserrat", 14);
    footerFont.setItalic(true);                    // Set italic style after construction

    // Get page dimensions
    QRect pageRect = printer.pageRect(QPrinter::DevicePixel).toRect();
    int pageWidth = pageRect.width();
    int pageHeight = pageRect.height();
    int margin = 150;  // Increased margin for a more spacious look (from previous design)
    int contentWidth = pageWidth - 2 * margin;
    int yPos = margin;

    // Increase header dimensions by 1000 (matching original placement)
    int baseHeaderWidth = contentWidth;
    int baseHeaderHeight = 150;
    int headerWidth = baseHeaderWidth + 1000;
    int headerHeight = baseHeaderHeight + 1000;

    // Cap header width to page boundaries
    if (headerWidth > pageWidth - margin) {
        headerWidth = pageWidth - margin - 20;
    }

    // Draw radial gradient background for header
    QRadialGradient radialGradient(pageWidth / 2, yPos, 400, pageWidth / 2, yPos);
    radialGradient.setColorAt(0, QColor(230, 240, 250));  // Light teal center
    radialGradient.setColorAt(1, QColor(163, 191, 250));  // Soft cyan edge
    painter.setBrush(QBrush(radialGradient));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(margin, yPos - 75, headerWidth, headerHeight, 40, 40);

    // Draw event name with shadow effect
    painter.setFont(nameFont);
    painter.setPen(QColor(0, 109, 119));  // Deep teal
    QRect nameRect(margin, yPos - 25, headerWidth, headerHeight - 50);
    painter.drawText(nameRect, Qt::AlignCenter, eventName);
    painter.setPen(Qt::gray);  // Shadow effect
    painter.drawText(nameRect.translated(3, 3), Qt::AlignCenter, eventName);  // Enhanced shadow
    yPos += headerHeight;

    // Draw modern underline with gradient
    QLinearGradient underlineGradient(margin + 100, yPos, margin + headerWidth - 100, yPos);
    underlineGradient.setColorAt(0, QColor(0, 109, 119));
    underlineGradient.setColorAt(1, QColor(163, 191, 250));
    painter.setPen(QPen(QBrush(underlineGradient), 6));
    painter.drawLine(margin + 100, yPos, margin + headerWidth - 100, yPos);
    yPos += 50;  // Matching original placement

    // Event details with aesthetic styling, including time
    QStringList details;
    details << "Date et heure de début: " + startDateTime.toString("dd/MM/yyyy HH:mm")
            << "Date et heure de fin: " + endDateTime.toString("dd/MM/yyyy HH:mm")
            << "Lieu: " + venue
            << "Capacité: " + QString::number(capacity)
            << "Sponsors: " + sponsors
            << "Budget: " + QString::number(budget, 'f', 2) + " TND";

    painter.setFont(textFont);
    QFontMetrics fm(textFont);
    int lineHeight = fm.height() * 1.5;  // Increased line height for better spacing

    // Detail box dimensions (increased by 1000, matching original placement)
    int baseBoxWidth = contentWidth - 40;
    int baseBoxHeight = lineHeight * 3 + 40;
    int boxWidth = baseBoxWidth + 1000;
    int boxHeight = baseBoxHeight + 1000;

    if (boxWidth > pageWidth - margin) {
        boxWidth = pageWidth - margin - 20;
    }

    painter.setPen(QColor(74, 74, 74));  // Warm gray
    for (const QString& detail : details) {
        QRect detailRect(margin, yPos, boxWidth, boxHeight);
        painter.setBrush(QBrush(QColor(245, 248, 250, 80)));  // Light background with more opacity
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(detailRect, 20, 20);
        painter.setPen(QColor(74, 74, 74));
        painter.drawText(detailRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, detail);

        if (yPos + boxHeight > pageHeight - 150) {
            printer.newPage();
            yPos = margin;
            // Redraw larger header on new page
            painter.setBrush(QBrush(radialGradient));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(margin, yPos - 75, headerWidth, headerHeight, 40, 40);
            painter.setFont(nameFont);
            painter.setPen(QColor(0, 109, 119));
            painter.drawText(nameRect, Qt::AlignCenter, eventName);
            painter.setPen(Qt::gray);
            painter.drawText(nameRect.translated(3, 3), Qt::AlignCenter, eventName);  // Shadow
            yPos += headerHeight;
            painter.setPen(QPen(QBrush(underlineGradient), 6));
            painter.drawLine(margin + 100, yPos, margin + headerWidth - 100, yPos);
            yPos += 50;
            painter.setFont(textFont);
            painter.setPen(QColor(74, 74, 74));
        }

        yPos += boxHeight + 30;  // Matching original placement
    }

    // Draw decorative footer
    painter.setBrush(QBrush(QColor(230, 240, 250, 120)));  // Enhanced footer background
    painter.setPen(Qt::NoPen);
    painter.drawRect(margin, pageHeight - 150, contentWidth, 100);
    painter.setFont(footerFont);
    painter.setPen(Qt::gray);
    QString footer = "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(margin + 20, pageHeight - 120, contentWidth - 40, 50, Qt::AlignLeft, footer);

    // Draw enhanced watermark
    QFont watermarkFont("Montserrat", 60, QFont::Light);
    painter.setFont(watermarkFont);
    QLinearGradient watermarkGradient(0, 0, pageWidth, pageHeight);
    watermarkGradient.setColorAt(0, QColor(0, 109, 119, 15));
    watermarkGradient.setColorAt(1, QColor(163, 191, 250, 5));
    painter.setPen(QPen(watermarkGradient, 2));
    painter.save();
    painter.translate(pageWidth / 2, pageHeight / 2);
    painter.rotate(-45);
    painter.drawText(-pageWidth / 2, -70, pageWidth, 140, Qt::AlignCenter, "Événement");
    painter.restore();

    painter.end();
    QMessageBox::information(this, "Succès", "L'événement a été exporté en PDF avec succès");
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);
}


void MainWindow::on_pushButton_search_clicked()
{
    QString searchText = ui->lineEdit_search_3->text().trimmed();
    evenement e;
    if (searchText.isEmpty()) {
        // If search field is empty, show all events
        ui->tableView4_3->setModel(e.afficher());
    } else {
        // Filter events by name
        ui->tableView4_3->setModel(e.searchByName(searchText));
    }
}

void MainWindow::on_lineEdit_search_3_textChanged(const QString &text)
{
    evenement e;
    if (text.trimmed().isEmpty()) {
        // If search field is empty, show all events
        ui->tableView4_3->setModel(e.afficher());
    } else {
        // Filter events by name
        ui->tableView4_3->setModel(e.searchByName(text.trimmed()));
    }
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);
}

#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QStringList>
#include "HistoryDialog.h" // Assuming this is the header for HistoryDialog
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QStringList>
#include "HistoryDialog.h" // Assuming this is the header for HistoryDialog

void MainWindow::on_pushButton_historique_3_clicked()
{
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "VERSIONS_STARTTIME, "
        "VERSIONS_OPERATION, "
        "ID_EV, NOM, DATE_DEBUT, DATE_FIN, LIEU, CAPACITE, SPONSORS, BUDGET "
        "FROM EVENEMENTS "
        "VERSIONS BETWEEN SCN MINVALUE AND MAXVALUE "
        "WHERE versions_operation IN ('U', 'D', 'I') "
        "ORDER BY ID_EV, VERSIONS_STARTTIME"
        );

    QStringList historyList;
    QMap<int, QVariantMap> previousState;  // To compare previous state

    if (query.exec()) {
        while (query.next()) {
            QString op = query.value("VERSIONS_OPERATION").toString();
            QString time = query.value("VERSIONS_STARTTIME").toDateTime().toString("yyyy-MM-dd HH:mm:ss");
            int id = query.value("ID_EV").toInt();
            QString nom = query.value("NOM").toString();

            QVariantMap current;
            current["NOM"] = query.value("NOM");
            current["DATE_DEBUT"] = query.value("DATE_DEBUT");
            current["DATE_FIN"] = query.value("DATE_FIN");
            current["LIEU"] = query.value("LIEU");
            current["CAPACITE"] = query.value("CAPACITE");
            current["SPONSORS"] = query.value("SPONSORS");
            current["BUDGET"] = query.value("BUDGET");

            if (op == "I") {
                historyList << QString("Note: Event '%1' (ID: %2) added on %3").arg(nom).arg(id).arg(time);
            } else if (op == "D") {
                historyList << QString("Note: Event '%1' (ID: %2) deleted on %3").arg(nom).arg(id).arg(time);
            } else if (op == "U") {
                QVariantMap prev = previousState.value(id);
                int changeCount = 0;

                // Count changes to summarize
                for (const auto& key : current.keys()) {
                    if (prev.contains(key) && prev[key] != current[key]) {
                        changeCount++;
                    }
                }

                if (changeCount > 0) {
                    QString summary = QString("Note: Event '%1' (ID: %2) modified on %3 - %4 field%5 updated")
                    .arg(nom)
                        .arg(id)
                        .arg(time)
                        .arg(changeCount)
                        .arg(changeCount > 1 ? "s" : "");
                    historyList << summary;
                }
            }

            // Store current state for next comparison
            previousState[id] = current;
        }

        // Save history to a text file (event_history.txt)
        //QFile file("event_history.txt");
        QString filePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/event_history.txt";
        QFile file(filePath);

        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << "Event History Log - Generated on " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";
            out << "--------------------------------------------------\n";
            for (const QString& entry : historyList) {
                out << entry << "\n";
            }
            file.close();
            QMessageBox::information(this, "Succès", "History notes saved to event_history.txt");
        } else {
            QMessageBox::warning(this, "Erreur", "Failed to save history notes to event_history.txt: " + file.errorString());
        }

        // Display history in the popup
        HistoryDialog *dialog = new HistoryDialog(this);
        dialog->setHistory(historyList);
        dialog->exec();
    } else {
        qDebug() << "Erreur dans la requête:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Failed to retrieve history: " + query.lastError().text());
    }
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);
}
void MainWindow::onComboBox2IndexChanged(int index)
{
    evenement e;
    QSqlQueryModel* sortedModel = nullptr;

    if (index == 0) { // Tri par lieu (bloc A → B)
        sortedModel = e.trier(QString("lieu"), QString("ASC"));
    }
    else if (index == 1) { // Tri par sponsors (A → Z)
        sortedModel = e.trier(QString("sponsors"), QString("ASC"));
    }
    else if (index == 2) { // Tri par capacité (desc)
        sortedModel = e.trier(QString("capacite"), QString("DESC"));
    }

    if (sortedModel) {
        ui->tableView4_3->setModel(sortedModel);
    }
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);
}

void MainWindow::checkUpcomingEvents()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM IMENE.EVENEMENTS WHERE DATE_DEBUT >= :currentDate AND DATE_DEBUT <= :twoDaysLater");

    QDate currentDate = QDate::currentDate();
    QDate twoDaysLater = currentDate.addDays(2);

    // Debug: Print current date and two days later
    qDebug() << "Current Date: " << currentDate.toString("yyyy-MM-dd");
    qDebug() << "Two Days Later: " << twoDaysLater.toString("yyyy-MM-dd");

    query.bindValue(":currentDate", QVariant(currentDate));
    query.bindValue(":twoDaysLater", QVariant(twoDaysLater));

    if (query.exec()) {
        bool eventFound = false;
        QString allMessages;

        while (query.next()) {
            QString eventName = query.value("NOM").toString();
            QString eventDate = query.value("DATE_DEBUT").toString();
            QString message = "• L'événement '" + eventName + "' arrive le " + eventDate + " !\n";
            allMessages += message;
            eventFound = true;
        }

        if (!eventFound) {
            allMessages = "Aucun événement à venir dans les 2 prochains jours.";
        }

        showNotification(allMessages);

    } else {
        // Show detailed error message from the database query
        QString error = query.lastError().text();
        QMessageBox::critical(this, "Erreur de la requête", "Erreur lors de l'exécution de la requête: " + error);
    }
}



void MainWindow::showNotification(const QString &message)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Notification");
    dialog->setStyleSheet(R"(
        QDialog {
            background-color: #f9f9f9;
            border-radius: 15px;
        }
        QLabel {
            font-size: 14px;
            color: #3e5879;
        }
        QPushButton {
            background-color: #3e5879;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 6px 14px;
        }
        QPushButton:hover {
            background-color: #5c759a;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel("Voici les événements à venir :", dialog);
    layout->addWidget(label);

    QStringList lines = message.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QLabel *msg = new QLabel(line, dialog);
        layout->addWidget(msg);
    }

    QPushButton *okBtn = new QPushButton("OK");
    layout->addWidget(okBtn);
    connect(okBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec(); // Affiche le popup
    connect(ui->tableView4_3->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onRowSelected);
}

void MainWindow::afficherRessources()
{
    QSqlQuery query("SELECT IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM IMENE.RESSOURCES");

    ui->tableWidget_3->setColumnCount(9);
    ui->tableWidget_3->setHorizontalHeaderLabels({"ID", "Nom", "Type", "État", "Quantité", "Prix", "Localisation", "Date Achat", "Date Maintenance"});

    ui->tableWidget_3->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget_3->insertRow(row);
        for (int col = 0; col < 9; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // ID non modifiable
            ui->tableWidget_3->setItem(row, col, item);
        }
        row++;
    }

    ui->tableWidget_3->setColumnHidden(0, true);  // cacher la colonne ID
}


void MainWindow::ajouterRessource()
{
    // Vérifie que tous les champs nécessaires sont remplis
    if (ui->lineEdit_nomres->text().isEmpty() ||
        ui->comboBox_type_3->currentText().isEmpty() ||
        ui->comboBox_etat_3->currentText().isEmpty() ||
        ui->lineEdit_quantres->text().isEmpty() ||
        ui->lineEdit_prixres->text().isEmpty() ||
        ui->lineEdit_locares->text().isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis.");
        return;
    }

    // Vérifie la validité des champs numériques
    bool ok;
    int quantite = ui->lineEdit_quantres->text().toInt(&ok);
    if (!ok || quantite < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide (doit être un entier positif).");
        return;
    }

    float prix = ui->lineEdit_prixres->text().toFloat(&ok);
    if (!ok || prix < 0) {
        QMessageBox::warning(this, "Erreur", "Prix invalide (doit être un nombre positif).");
        return;
    }

    QDate dateAchat = ui->dateEdit_dateacha->date();
    QDate dateMaintenance = ui->dateEdit_datemaint->date();

    if (dateMaintenance < dateAchat) {
        QMessageBox::warning(this, "Erreur", "La date de maintenance ne peut pas être antérieure à la date d'achat.");
        return;
    }

    // Préparation de la requête SQL d'insertion
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO IMENE.RESSOURCES (IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM)
        VALUES (RESSOURCES_SEQ.NEXTVAL, :nom, :type, :etat, :quantite, :prix, :localisation,
                TO_DATE(:dateAchat, 'DD-MM-YYYY'), TO_DATE(:dateMaintenance, 'DD-MM-YYYY'))
    )");

    // Liaison des valeurs
    query.bindValue(":nom", ui->lineEdit_nomres->text().trimmed());
    query.bindValue(":type", ui->comboBox_type_3->currentText());
    query.bindValue(":etat", ui->comboBox_etat_3->currentText());
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);
    query.bindValue(":localisation", ui->lineEdit_locares->text().trimmed());
    query.bindValue(":dateAchat", dateAchat.toString("dd-MM-yyyy"));
    query.bindValue(":dateMaintenance", dateMaintenance.toString("dd-MM-yyyy"));

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout : " + query.lastError().text());
        qDebug() << "Erreur SQL : " << query.lastError().text();
        return;
    }

    // ✅ Génération d’un QR code avec le nom
    QString chemin = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                     + "/" + ui->lineEdit_nomres->text().trimmed() + "_QRCode.png";
    genererQRCode(ui->lineEdit_nomres->text().trimmed(), chemin);

    QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès.");

    // Rafraîchissement de l’interface
    afficherRessources();
    afficherDatesMaintenance();
    TrierParDisponibilite();

    // Nettoyage des champs
    ui->lineEdit_nomres->clear();
    ui->comboBox_type_3->setCurrentIndex(-1);
    ui->comboBox_etat_3->setCurrentIndex(-1);
    ui->lineEdit_quantres->clear();
    ui->lineEdit_prixres->clear();
    ui->lineEdit_locares->clear();
    ui->dateEdit_dateacha->clear();
    ui->dateEdit_datemaint->clear();
}



// ✅ 2. modifierRessource corrigée :
void MainWindow::modifierRessource()
{
    int selectedRow = ui->tableWidget_3->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ressource à modifier !");
        return;
    }

    // Récupérer l'ID directement depuis la colonne cachée
    int idR = ui->tableWidget_3->item(selectedRow, 0)->text().toInt();

    // Récupérer les anciennes valeurs
    QString oldNom = ui->tableWidget_3->item(selectedRow, 1)->text();
    QString oldType = ui->tableWidget_3->item(selectedRow, 2)->text();
    QString oldEtat = ui->tableWidget_3->item(selectedRow, 3)->text();
    int oldQuantite = ui->tableWidget_3->item(selectedRow, 4)->text().toInt();
    float oldPrix = ui->tableWidget_3->item(selectedRow, 5)->text().toFloat();
    QString oldLocalisation = ui->tableWidget_3->item(selectedRow, 6)->text();
    QDate oldDateAchat = QDate::fromString(ui->tableWidget_3->item(selectedRow, 7)->text(), "yyyy-MM-dd");
    QDate oldDateMaintenance = QDate::fromString(ui->tableWidget_3->item(selectedRow, 8)->text(), "yyyy-MM-dd");

    // Nouvelles valeurs (ou anciennes si vide)
    QString nom = ui->lineEdit_nomres->text().trimmed().isEmpty() ? oldNom : ui->lineEdit_nomres->text().trimmed();
    QString type = ui->comboBox_type_3->currentText().isEmpty() ? oldType : ui->comboBox_type_3->currentText();
    QString etat = ui->comboBox_etat_3->currentText().isEmpty() ? oldEtat : ui->comboBox_etat_3->currentText();
    int quantite = ui->lineEdit_quantres->text().trimmed().isEmpty() ? oldQuantite : ui->lineEdit_quantres->text().trimmed().toInt();
    float prix = ui->lineEdit_prixres->text().trimmed().isEmpty() ? oldPrix : ui->lineEdit_prixres->text().trimmed().toFloat();
    QString localisation = ui->lineEdit_locares->text().trimmed().isEmpty() ? oldLocalisation : ui->lineEdit_locares->text().trimmed();
    QDate dateAchat = ui->dateEdit_dateacha->date().isValid() ? ui->dateEdit_dateacha->date() : oldDateAchat;
    QDate dateMaintenance = ui->dateEdit_datemaint->date().isValid() ? ui->dateEdit_datemaint->date() : oldDateMaintenance;

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

    // Nettoyage
    ui->lineEdit_nomres->clear();
    ui->comboBox_type_3->setCurrentIndex(-1);
    ui->comboBox_etat_3->setCurrentIndex(-1);
    ui->lineEdit_quantres->clear();
    ui->lineEdit_prixres->clear();
    ui->lineEdit_locares->clear();
    ui->dateEdit_dateacha->clear();
    ui->dateEdit_datemaint->clear();
}



void MainWindow::supprimerRessource()
{
    int selectedRow = ui->tableWidget_3->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ressource à supprimer !");
        return;
    }

    // ✅ On récupère l'ID de la ressource (colonne 0 cachée)
    int id = ui->tableWidget_3->item(selectedRow, 0)->text().toInt();

    // ✅ Pour affichage plus clair dans le message
    QString nom = ui->tableWidget_3->item(selectedRow, 1)->text();

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

    for (int row = 0; row < ui->tableWidget_3->rowCount(); ++row) {
        // ✅ Colonne 1 = Nom | Colonne 4 = Quantité
        QString nom = ui->tableWidget_3->item(row, 1)->text();
        int quantite = ui->tableWidget_3->item(row, 4)->text().toInt();
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
    int startX = (pageWidth - (ui->tableWidget_3->columnCount() * columnWidth)) / 2;

    QFont cellFont("Arial", 8);
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    painter.setPen(Qt::NoPen);

    for (int col = 0; col < ui->tableWidget_3->columnCount(); ++col) {
        QRect rect(startX + col * columnWidth, y, columnWidth, rowHeight);
        painter.setBrush(headerBgColor);
        painter.drawRoundedRect(rect, 5, 5);
        painter.setPen(headerTextColor);
        painter.drawText(rect, Qt::AlignCenter, ui->tableWidget_3->horizontalHeaderItem(col)->text());
        painter.setPen(Qt::NoPen);
    }

    y += rowHeight;

    // 📄 Données
    painter.setFont(cellFont);
    painter.setPen(borderColor);
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < ui->tableWidget_3->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget_3->columnCount(); ++col) {
            QRect rect(startX + col * columnWidth, y, columnWidth, rowHeight);
            painter.drawRect(rect);

            QTableWidgetItem *item = ui->tableWidget_3->item(row, col);
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
    QString choix = ui->comboBox_tri_3->currentText();
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
    QString queryStr = "SELECT IDR, NOMR, TYPER, ETATR, QUANTITER, PRIXR, LOCR, DAR, DM FROM IMENE.RESSOURCES" + conditionEtat + tri;
    QSqlQuery query;
    query.prepare(queryStr);
    if (!etatFiltre.isEmpty()) query.bindValue(":etat", etatFiltre);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération : " + query.lastError().text());
        return;
    }

    // 5. Affichage dans le tableau
    ui->tableWidget_3->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_3->insertRow(row);
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // ID non modifiable
            ui->tableWidget_3->setItem(row, col, item);
        }
        row++;
    }

    ui->tableWidget_3->setColumnHidden(0, true);  // Cacher colonne ID
}

void MainWindow::filtrerRessources()
{
    QString texteRecherche = ui->lineEdit_recherche_3->text().trimmed();

    for (int i = 0; i < ui->tableWidget_3->rowCount(); ++i) {
        bool afficher = false;

        for (int j = 0; j < ui->tableWidget_3->columnCount(); ++j) {
            QTableWidgetItem *item = ui->tableWidget_3->item(i, j);
            if (item && item->text().contains(texteRecherche, Qt::CaseInsensitive)) {
                afficher = true;
                break;
            }
        }

        ui->tableWidget_3->setRowHidden(i, !afficher);
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
    ui->calendar_maintenance_3->setSelectedDate(QDate::currentDate()); // optionnel

    // On vide les anciennes mises en forme
    QMap<QDate, QTextCharFormat> formats = ui->calendar_maintenance_3->dateTextFormat();
    for (auto date : formats.keys()) {
        ui->calendar_maintenance_3->setDateTextFormat(date, QTextCharFormat());
    }

    // On crée le style
    QTextCharFormat format;
    format.setBackground(Qt::blue); // ou QColor("#8cb4ff") si tu veux une autre couleur
    format.setForeground(Qt::white);
    format.setFontWeight(QFont::Bold);

    QSqlQuery query("SELECT DISTINCT DM FROM IMENE.RESSOURCES");

    while (query.next()) {
        QDate date = query.value(0).toDate();
        if (date.isValid()) {
            ui->calendar_maintenance_3->setDateTextFormat(date, format);
        }
    }
}
void MainWindow::afficherRessourcesPourDate(const QDate &date)
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
void MainWindow::on_pushButton_qr_3_clicked()
{
    int selectedRow = ui->tableWidget_3->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une ressource dans le tableau.");
        return;
    }

    // Récupération des données
    QString nom = ui->tableWidget_3->item(selectedRow, 1)->text();
    QString type = ui->tableWidget_3->item(selectedRow, 2)->text();
    QString etat = ui->tableWidget_3->item(selectedRow, 3)->text();
    QString quantite = ui->tableWidget_3->item(selectedRow, 4)->text();
    QString prix = ui->tableWidget_3->item(selectedRow, 5)->text();
    QString localisation = ui->tableWidget_3->item(selectedRow, 6)->text();
    QString dateAchat = ui->tableWidget_3->item(selectedRow, 7)->text();
    QString dateMaintenance = ui->tableWidget_3->item(selectedRow, 8)->text();

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

    query.prepare("UPDATE IMENE0RESSOURCES SET ETATR = 'En panne', DM = TO_DATE(:date, 'DD-MM-YYYY') WHERE IDR = :id");
    query.bindValue(":date", today);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✅ Ressource ID" << id << "mise à jour : En panne.";

        afficherRessources(); // Refresh du tableau

        // 1. On récupère le NOM de la ressource
        QSqlQuery getNameQuery;
        getNameQuery.prepare("SELECT NOMR FROM IMENE.RESSOURCES WHERE IDR = :id");
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
