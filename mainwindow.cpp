#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QPainter>
#include <QPdfWriter>
#include <QChartView>
#include <QSqlQuery>
#include <QMessageBox>
#include <QColor>
#include <QFont>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include<QBarSeries>
#include <QCryptographicHash>
#include <QtCharts/QBarSet>  // Pour QBarSet
#include <QDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ... (connexions existantes)



    ui->setupUi(this);
    connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::onvaliderClicked);
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::onvalClicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::ondelClicked);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::onupClicked);
    connect(ui->tw, &QTableWidget::cellClicked, this, &MainWindow::populateFields);
    // Connecter le signal currentIndexChanged pour détecter le changement dans le QComboBox
    connect(ui->critere, &QComboBox::currentIndexChanged, this, &MainWindow::onTriClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::onpdfclicked);
    connect(ui->statbutton, &QPushButton::clicked, this, &MainWindow::on_statbutton_clicked);
    verticalLayout = new QVBoxLayout(ui->centralWidget);
    connect(ui->showPasswordCheckbox, &QCheckBox::toggled, this, &MainWindow::onShowPasswordToggled);
    /*connect(ui->historiqueButton, &QPushButton::clicked, this, &MainWindow::onHistoriqueClicked);
    connect(ui->historiqueButton, &QPushButton::clicked, this, &MainWindow::onHistoriqueClicked);*/

    serial = new QSerialPort(this);
    serial->setPortName("COM4"); // Mettre le bon port (sur Linux ça serait /dev/ttyUSB0)
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    /*if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::onSerialDataReceived);
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série !");
    }*/


    /*QPushButton *forgotPasswordButton = new QPushButton("Mot de passe oublié ?", this);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &MainWindow::onPasswordForgotClicked);
    verticalLayout->addWidget(forgotPasswordButton);

    if (ui->verticalLayout) {
        ui->verticalLayout->addWidget(forgotPasswordButton);
    } else {
        // Créer un layout si nécessaire
        QVBoxLayout *layout = new QVBoxLayout(ui->centralWidget);
        layout->addWidget(forgotPasswordButton);
        ui->centralWidget->setLayout(layout);
    }*/

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onTriClicked()
{
    QString critere = ui->critere->currentText();
    trierEmployes(critere);
}


void MainWindow::onvaliderClicked()
{
    QString nom = ui->nom->text().trimmed();
    QString poste = ui->poste->currentText().trimmed();
    QString numt = ui->numt->text().trimmed();
    QString mail = ui->mail->text().trimmed();
    QString pwd = ui->pwd->text();
    QString dn = ui->dn->text().trimmed();
    QString gnr = ui->gnr->currentText().trimmed();
    QString question = ui->questionCombo->currentText();
    QString reponse = ui->reponseInput->text().trimmed();
   //QString hashedReponse = QCryptographicHash::hash(reponse.toUtf8(), QCryptographicHash::Sha256).toHex();
    // Validations
    if(nom.isEmpty() || poste.isEmpty() || numt.isEmpty() || mail.isEmpty() || pwd.isEmpty() || dn.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires");
        return;
    }

    if(!QRegularExpression("^[0-9]{8}$").match(numt).hasMatch()){
        QMessageBox::warning(this, "Erreur", "Numéro de téléphone invalide");
        return;
    }

    if(!QRegularExpression(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)").match(mail).hasMatch()){
        QMessageBox::warning(this, "Erreur", "Email invalide");
        return;
    }

    QDate dateNaissance = QDate::fromString(dn, "dd/MM/yyyy");
    if(!dateNaissance.isValid() || dateNaissance > QDate::currentDate().addYears(-20)){
        QMessageBox::warning(this, "Erreur", "L'employé doit avoir au moins 20 ans");
        return;
    }

    // Hachage mot de passe

    // Insertion
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEE (NOM, POSTE, NUMTEL, EMAIL, PASSWORD, GENRE, DN, QUESTION, REPONSE) "
                  "VALUES (:nom, :poste, :numt, :mail, :pwd, :gnr, :dn, :question, :reponse)");
    query.bindValue(":nom", nom);
    query.bindValue(":poste", poste);
    query.bindValue(":numt", numt);
    query.bindValue(":mail", mail);
    query.bindValue(":pwd", pwd);
    query.bindValue(":gnr", gnr);
    query.bindValue(":dn", dn);
    query.bindValue(":question", question);
    query.bindValue(":reponse", reponse);

    if(!query.exec()){
        QMessageBox::critical(this, "Erreur", "Erreur d'insertion: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès");
        refreshTable();
    }
    enregistrerHistorique("admin", "Ajout employé", nom);

}

void MainWindow::onvalClicked()
{
    val();
}

void MainWindow::val()
{
    QSqlQuery query("SELECT NOM, POSTE, NUMTEL, EMAIL, PASSWORD,GENRE , DN FROM EMPLOYEE");

    ui->tw->setRowCount(0); // Vider le tableau
    int row = 0;

    while (query.next()) {
        ui->tw->insertRow(row);

        ui->tw->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tw->setItem(row, 1, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tw->setItem(row, 2, new QTableWidgetItem(query.value("NUMTEL").toString()));
        ui->tw->setItem(row, 3, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tw->setItem(row, 4, new QTableWidgetItem(query.value("PASSWORD").toString()));
        ui->tw->setItem(row, 5, new QTableWidgetItem(query.value("GENRE").toString()));
        ui->tw->setItem(row, 6, new QTableWidgetItem(query.value("DN").toString()));

        row++;
    }


}

void MainWindow::ondelClicked()
{
    QList<QTableWidgetItem *> selectedItems = ui->tw->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Delete", "No item selected for deletion.");
        return;
    }

    for (QTableWidgetItem *item : selectedItems) {
        int row = item->row();
        QString nom = ui->tw->item(row, 0)->text();  // << Déclaré ici

        QSqlQuery query;
        query.prepare("DELETE FROM EMPLOYEE WHERE NOM = :nom");
        query.bindValue(":nom", nom);

        if (query.exec()) {
            ui->tw->removeRow(row);
            QMessageBox::information(this, "Delete", "Data deleted successfully.");

            // ✅ Appel ici, dans le bon scope
            enregistrerHistorique("admin", "Suppression employé", nom);
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data: " + query.lastError().text());
        }
    }
    for (QTableWidgetItem *item : selectedItems) {
        int row = item->row();
        QString nom = ui->tw->item(row, 0)->text();  // << Déclaré ici

        QSqlQuery query;
        query.prepare("DELETE FROM EMPLOYEE WHERE NOM = :nom");
        query.bindValue(":nom", nom);

        if (query.exec()) {
            ui->tw->removeRow(row);
            QMessageBox::information(this, "Delete", "Data deleted successfully.");

            // ✅ Appel ici, dans le bon scope
            enregistrerHistorique("admin", "Suppression employé", nom);
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data: " + query.lastError().text());
        }
    }


}
void MainWindow::onupClicked()
{
    int row = ui->tw->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Update", "No item selected for updating.");
        return;
    }

    QString nom = ui->nom->text();
    QString poste = ui->poste->currentText();
    QString numt = ui->numt->text();
    QString mail = ui->mail->text();
    QString pwd = ui->pwd->text();
    QString gnr = ui->gnr->currentText();
    QString dn = ui->dn->text();

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET NOM = :nom, POSTE = :poste, NUMTEL= :numt, EMAIL = :mail, PASSWORD = :pwd,GENRE = :gnr, DN= :dn WHERE NOM = :nom");
    query.bindValue(":nom", nom);
    query.bindValue(":poste", poste);
    query.bindValue(":numt", numt);
    query.bindValue(":mail", mail);
    query.bindValue(":pwd", pwd);
    query.bindValue(":gnr", gnr);
    query.bindValue(":dn", dn);

    if (gnr != "Masculin" && gnr != "Féminin") {
        QMessageBox::warning(this, "Erreur", "Le genre doit être soit 'Masculin' soit 'Féminin' !");
        return;
    }



    // Vérification des champs vides
    if (nom.isEmpty() || poste.isEmpty() || numt.isEmpty() || mail.isEmpty() || pwd.isEmpty() || dn.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    // Vérification du numéro de téléphone (doit contenir uniquement des chiffres et avoir une longueur valide)
    QRegularExpression numRegex("^[0-9]{8}$");
    if (!numRegex.match(numt).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir uniquement des chiffres (8 caractères) !");
        return;
    }

    // Vérification du format de l'email
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
    if (!emailRegex.match(mail).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Adresse email invalide !");
        return;
    }

    // Vérification de la complexité du mot de passe (au moins 6 caractères avec une lettre et un chiffre)
    QRegularExpression pwdRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
    if (!pwdRegex.match(pwd).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 6 caractères, avec au moins une lettre et un chiffre !");
        return;
    }
    // Vérification de la date de naissance (au moins 20 ans)
    QDate dateNaissance = QDate::fromString(dn, "dd/MM/yyyy"); // Assurez-vous que le format correspond à celui de votre interface
    if (!dateNaissance.isValid()) {
        QMessageBox::warning(this, "Erreur", "Date de naissance invalide !");
        return;
    }

    QDate aujourdhui = QDate::currentDate();
    QDate dateLimite = aujourdhui.addYears(-20); // Date il y a 20 ans

    if (dateNaissance > dateLimite) {
        QMessageBox::warning(this, "Erreur", "L'employé doit avoir au moins 20 ans !");
        return;
    }
    // Vérification des doublons
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE NOM = :nom AND POSTE = :poste AND NUMTEL = :numt AND EMAIL = :mail AND GENRE = :gnr AND DN = :dn");
    checkQuery.bindValue(":nom", nom);
    checkQuery.bindValue(":poste", poste);
    checkQuery.bindValue(":numt", numt);
    checkQuery.bindValue(":mail", mail);
    checkQuery.bindValue(":gnr", gnr);
    checkQuery.bindValue(":dn", dn);

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "Erreur", "Un employé avec les mêmes informations existe déjà !");
            return;
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la vérification des doublons : " + checkQuery.lastError().text());
        return;
    }


    if (query.exec()) {
        QMessageBox::information(this, "Update", "Data updated successfully.");
        val(); // Refresh table
    } else {
        QMessageBox::critical(this, "Error", "Failed to update data: " + query.lastError().text());
    }
    enregistrerHistorique("admin", "Modification employé", nom);


}

void MainWindow::populateFields(int row)
{
    ui->nom->setText(ui->tw->item(row, 0)->text());
    ui->poste->setCurrentText(ui->tw->item(row, 1)->text());
    ui->numt->setText(ui->tw->item(row, 2)->text());
    ui->mail->setText(ui->tw->item(row, 3)->text());
    ui->pwd->setText(ui->tw->item(row, 4)->text());
    ui->gnr->setCurrentText(ui->tw->item(row, 5)->text());
    ui->dn->setDate(QDate::fromString(ui->tw->item(row, 6)->text(), "dd/MM/yyyy"));
}

void MainWindow::trierEmployes(const QString &critere)
{
    QString queryStr = "SELECT NOM, POSTE, NUMTEL, EMAIL, PASSWORD, GENRE, DN FROM EMPLOYEE";

    if (critere == "Nom") {
        queryStr += " ORDER BY NOM ASC";
    } else if (critere == "Poste") {
        queryStr += " ORDER BY POSTE ASC";
    } else if (critere == "Date de naissance") {
        queryStr += " ORDER BY TO_DATE(DN, 'DD/MM/YYYY') ASC";
    }


    QSqlQuery query(queryStr);

    ui->tw->setColumnCount(7);
    QStringList headers = {"Nom", "Poste", "Téléphone", "Email", "Mot de passe", "Genre", "Date de naissance"};
    ui->tw->setHorizontalHeaderLabels(headers);
    ui->tw->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tw->insertRow(row);
        ui->tw->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tw->setItem(row, 1, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tw->setItem(row, 2, new QTableWidgetItem(query.value("NUMTEL").toString()));
        ui->tw->setItem(row, 3, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tw->setItem(row, 4, new QTableWidgetItem(query.value("PASSWORD").toString()));
        ui->tw->setItem(row, 5, new QTableWidgetItem(query.value("GENRE").toString()));
        ui->tw->setItem(row, 6, new QTableWidgetItem(query.value("DN").toString()));
        row++;
    }
}

void MainWindow::rechercherEmploye(const QString &criteres)
{
    QString queryStr = "SELECT NOM, POSTE, NUMTEL, EMAIL, PASSWORD, GENRE, DN FROM EMPLOYEE WHERE NOM LIKE :criteres OR POSTE LIKE :criteres";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":criteres", "%" + criteres + "%"); // Le "%" permet de rechercher des correspondances partielles

    if (query.exec()) {
        ui->tw->setRowCount(0); // Vider le tableau avant de remplir avec les nouveaux résultats
        int row = 0;
        while (query.next()) {
            ui->tw->insertRow(row);
            ui->tw->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
            ui->tw->setItem(row, 1, new QTableWidgetItem(query.value("POSTE").toString()));
            ui->tw->setItem(row, 2, new QTableWidgetItem(query.value("NUMTEL").toString()));
            ui->tw->setItem(row, 3, new QTableWidgetItem(query.value("EMAIL").toString()));
            ui->tw->setItem(row, 4, new QTableWidgetItem(query.value("PASSWORD").toString()));
            ui->tw->setItem(row, 5, new QTableWidgetItem(query.value("GENRE").toString()));
            ui->tw->setItem(row, 6, new QTableWidgetItem(query.value("DN").toString()));
            row++;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la recherche : " + query.lastError().text());
    }
}
void MainWindow::onSearchClicked()
{
    QString searchText = ui->search->text().trimmed();

    QSqlQuery query;
    if (searchText.isEmpty()) {
        // Si le champ est vide, on affiche tout
        query.prepare("SELECT NOM, POSTE, NUMTEL, EMAIL, PASSWORD, GENRE, DN FROM EMPLOYEE");
    } else {
        // Sinon, on fait une recherche partielle sur plusieurs champs (LIKE)
        query.prepare(R"(
    SELECT NOM, POSTE, NUMTEL, EMAIL, PASSWORD, GENRE, DN
    FROM EMPLOYEE
    WHERE UPPER(NOM) LIKE UPPER(:search)
       OR UPPER(POSTE) LIKE UPPER(:search)
       OR UPPER(EMAIL) LIKE UPPER(:search)
       OR UPPER(NUMTEL) LIKE UPPER(:search)
       OR UPPER(DN) LIKE UPPER(:search)
)");
        query.bindValue(":search", "%" + searchText + "%");

    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Recherche impossible : " + query.lastError().text());
        return;
    }

    // On vide le tableau et on affiche les résultats
    ui->tw->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tw->insertRow(row);
        ui->tw->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tw->setItem(row, 1, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tw->setItem(row, 2, new QTableWidgetItem(query.value("NUMTEL").toString()));
        ui->tw->setItem(row, 3, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tw->setItem(row, 4, new QTableWidgetItem(query.value("PASSWORD").toString()));
        ui->tw->setItem(row, 5, new QTableWidgetItem(query.value("GENRE").toString()));
        ui->tw->setItem(row, 6, new QTableWidgetItem(query.value("DN").toString()));
        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "Recherche", "Aucun résultat trouvé.");
    }
}



void MainWindow::searchEmployes(const QString &searchText)
{
    // Exemple : chercher dans la base de données ou filtrer la table en fonction du texte
    QString queryStr = QString("SELECT * FROM EMPLOYEE WHERE NOM LIKE '%%1%' OR POSTE LIKE '%%1%'")
                           .arg(searchText);

    QSqlQuery query;
    query.exec(queryStr);

    // Vider le tableau avant d'afficher les nouveaux résultats
    ui->tw->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tw->insertRow(row);
        ui->tw->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tw->setItem(row, 1, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tw->setItem(row, 2, new QTableWidgetItem(query.value("NUMTEL").toString()));
        ui->tw->setItem(row, 3, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tw->setItem(row, 4, new QTableWidgetItem(query.value("PASSWORD").toString()));
        ui->tw->setItem(row, 5, new QTableWidgetItem(query.value("GENRE").toString()));
        ui->tw->setItem(row, 6, new QTableWidgetItem(query.value("DN").toString()));
        row++;
    }
}

void MainWindow::refreshTable()
{
    // Exemple d'implémentation simple pour rafraîchir la table sans filtres
    // Ce code dépend de la manière dont tu gères les employés et les données
    ui->tw->setRowCount(0);  // Vider les lignes de la table
    QSqlQuery query("SELECT * FROM EMPLOYEE");  // Récupérer tous les employés

    int row = 0;
    while (query.next()) {
        ui->tw->insertRow(row);
        ui->tw->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tw->setItem(row, 1, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tw->setItem(row, 2, new QTableWidgetItem(query.value("NUMTEL").toString()));
        ui->tw->setItem(row, 3, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tw->setItem(row, 4, new QTableWidgetItem(query.value("PASSWORD").toString()));
        ui->tw->setItem(row, 5, new QTableWidgetItem(query.value("GENRE").toString()));
        ui->tw->setItem(row, 6, new QTableWidgetItem(query.value("DN").toString()));
        row++;
    }
}


void MainWindow::onpdfclicked()
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

    // 🟤 Fond beige
    QRectF fullPage(0, 0, writer.width(), writer.height());
    painter.fillRect(fullPage, QColor("#f5ede0"));

    int pageWidth = writer.width();
    int startX = 200;
    int startY = 300;
    int rowHeight = 70;
    int columnWidth = 280;

    // 🔵 Titre avec fond bleu
    int titleWidth = 900;
    int titleHeight = 90;
    int titleX = (pageWidth - titleWidth) / 2;
    QRect titleRect(titleX, 85, titleWidth, titleHeight);

    painter.setBrush(QColor("#2f466c"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(titleRect, 15, 15);

    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::white);
    painter.drawText(titleRect, Qt::AlignCenter, "🗂️ Liste des Boutiques");

    // 📅 Date centrée
    painter.setFont(QFont("Arial", 9));
    painter.setPen(Qt::black);
    QString dateText = QLocale(QLocale::French).toString(QDate::currentDate(), "dddd d MMMM yyyy")
                       + " à " + QTime::currentTime().toString("hh:mm");
    painter.drawText(QRect(0, 160, pageWidth, 20), Qt::AlignCenter, dateText);

    // 🟦 En-têtes
    QFont headerFont("Arial", 11, QFont::Bold);
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    QStringList headers = {"Nom", "Poste", "Numtel", "Email", "Password", "Genre", "DN"};

    for (int i = 0; i < headers.size(); ++i) {
        QRect rect(startX + i * columnWidth, startY, columnWidth, rowHeight);
        painter.setBrush(QColor("#2f466c"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);
        painter.setPen(Qt::white);
        painter.drawText(rect, Qt::AlignCenter, headers[i]);
    }

    // 📄 Données
    QFont contentFont("Arial", 10);
    painter.setFont(contentFont);
    painter.setPen(Qt::black);

    for (int row = 0; row < ui->tw->rowCount(); ++row) {
        for (int col = 0; col < ui->tw->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tw->item(row, col);
            QString text = item ? item->text() : "";
            QRect rect(startX + col * columnWidth, startY + (row + 1) * rowHeight, columnWidth, rowHeight);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, text);
        }
    }

    painter.end();
    QMessageBox::information(this, "PDF", "PDF généré avec succès !");
}


void MainWindow::on_statbutton_clicked()
{
    qDebug() << "Bouton cliqué !"; // Ajoutez cette ligne

    QTabWidget *tabWidget = new QTabWidget();

    struct StatInfo {
        QString title;
        QString field;
    };

    QList<StatInfo> stats = {
        {"Répartition par poste", "POSTE"},
        {"Répartition par genre", "GENRE"},
        {"Répartition par année", "EXTRACT(YEAR FROM TO_DATE(DN, 'DD/MM/YYYY'))"}  // Pour PostgreSQL/ORACLE
        // Ou pour SQLite : "strftime('%Y', substr(DN, 7, 4) || '-' || substr(DN, 4, 2) || '-' || substr(DN, 1, 2))"
    };

    QStringList colors = {
        "#ff6f61", "#6b5b95", "#88b04b", "#f7cac9",
        "#92a8d1", "#955251", "#b565a7", "#009688",
        "#f4b400", "#607d8b"
    };

    for (const StatInfo &stat : stats)
    {
        QPieSeries *series = new QPieSeries();

        QSqlQuery query;
        QString queryString = "SELECT " + stat.field + ", COUNT(*) AS count FROM EMPLOYEE GROUP BY " + stat.field;
        query.prepare(queryString);

        if (query.exec()) {
            while (query.next()) {
                QString label = query.value(0).toString();
                int count = query.value(1).toInt();
                if (!label.isEmpty()) {
                    QPieSlice *slice = series->append(label, count);
                    slice->setLabelVisible(true);
                    slice->setExploded(true);
                    slice->setLabel(QString("%1 (%2)").arg(label).arg(count));
                }
            }
        }

        int i = 0;
        for (QPieSlice *slice : series->slices()) {
            slice->setColor(QColor(colors[i % colors.size()]));
            i++;
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(stat.title);
        chart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->legend()->setFont(QFont("Segoe UI", 10));

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(chartView);
        tab->setLayout(layout);

        tabWidget->addTab(tab, stat.title);
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques ");
    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->addWidget(tabWidget);
    dialog->setLayout(dialogLayout);
    dialog->resize(800, 500);
    dialog->exec();
}
// Implémentation :
void MainWindow::onDashboardClicked() {
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // 1. Graphique des postes (camembert)
    QPieSeries *posteSeries = new QPieSeries();
    QSqlQuery posteQuery("SELECT POSTE, COUNT(*) FROM EMPLOYEE GROUP BY POSTE");
    while (posteQuery.next()) {
        posteSeries->append(posteQuery.value(0).toString(), posteQuery.value(1).toInt());
    }

    // 2. Graphique des âges (histogramme)
    QBarSeries *ageSeries = new QBarSeries();
    QBarSet *ages = new QBarSet("Tranches d'âge");
    *ages << calculateEmployeesInRange(20, 30) << calculateEmployeesInRange(30, 40);  // À implémenter
    ageSeries->append(ages);

    // Ajout des graphiques au layout
    QChartView *posteView = new QChartView();
    posteView->setChart(new QChart());
    posteView->chart()->addSeries(posteSeries);
    layout->addWidget(posteView);

    dialog->exec();
}

int MainWindow::calculateEmployeesInRange(int minAge, int maxAge) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYEE "
                  "WHERE EXTRACT(YEAR FROM AGE(TO_DATE(DN, 'DD/MM/YYYY'))) BETWEEN ? AND ?");
    query.addBindValue(minAge);
    query.addBindValue(maxAge);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// Dialogue de réinitialisation
void MainWindow::showPasswordResetDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Réinitialisation du mot de passe");
    QFormLayout layout(&dialog);

    // Champ Nom
    QLineEdit nomInput;
    layout.addRow("Nom :", &nomInput);

    // ComboBox pour les questions secrètes
    QComboBox questionCombo;
    questionCombo.addItems(getSecretQuestions());
    layout.addRow("Question secrète :", &questionCombo);

    // Champ Réponse
    QLineEdit answerInput;
    layout.addRow("Réponse :", &answerInput);


    // Nouveau mot de passe
    QLineEdit newPasswordInput;
    newPasswordInput.setEchoMode(QLineEdit::Password);
    layout.addRow("Nouveau mot de passe :", &newPasswordInput);

    // Bouton Valider
    QPushButton validateButton("Valider");
    QObject::connect(&validateButton, &QPushButton::clicked, [&]() {
        if (verifySecretAnswer(nomInput.text().trimmed(),
                               questionCombo.currentText(),
                               answerInput.text().trimmed())) {
            updatePassword(nomInput.text().trimmed(), newPasswordInput.text().trimmed());
            QMessageBox::information(this, "Succès", "Mot de passe mis à jour !");
            dialog.close();
        } else {
            QMessageBox::warning(this, "Erreur", "Réponse incorrecte ou employé inconnu.");
        }
    });
    layout.addRow(&validateButton);

    dialog.exec();
}

// Récupère les questions disponibles
QStringList MainWindow::getSecretQuestions() {
    QStringList questions;
    QSqlQuery query("SELECT DISTINCT QUESTION FROM EMPLOYEE WHERE QUESTION IS NOT NULL");
    while (query.next()) {
        questions << query.value(0).toString();
    }
    return questions;
}


// Met à jour le mot de passe
void MainWindow::updatePassword(const QString &nom, const QString &newPassword) {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET PASSWORD = :pwd WHERE NOM = :nom");
    query.bindValue(":pwd", newPassword);
    query.bindValue(":nom", nom);
    query.exec();
}
void MainWindow::onPasswordForgotClicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Réinitialisation du mot de passe");

    QFormLayout *layout = new QFormLayout(dialog);

    QCheckBox *showPasswordCheckbox = new QCheckBox("Afficher le mot de passe");

    // Champ Nom

    QComboBox *nomCombo = new QComboBox(dialog);
    layout->addRow("Nom :", nomCombo);
    QSqlQuery query("SELECT DISTINCT NOM FROM EMPLOYEE");
    while (query.next()) {
        nomCombo->addItem(query.value(0).toString());
    }

    layout->addRow("Nom :", nomCombo);

    // ComboBox Questions
    QComboBox *questionCombo = new QComboBox(dialog);
    questionCombo->addItems({"Quel est le nom de votre premier animal ?", "Quelle est votre ville de naissance ?", "Quel était le nom de votre premier professeur ?"});
    layout->addRow("Question :", questionCombo);

    // Champ Réponse
    QLineEdit *answerInput = new QLineEdit(dialog);
    layout->addRow("Réponse :", answerInput);

    // Nouveau mot de passe
    QLineEdit *newPasswordInput = new QLineEdit(dialog);
    newPasswordInput->setEchoMode(QLineEdit::Password);
    layout->addRow("Nouveau mot de passe :", newPasswordInput);

    // Bouton Valider
    QPushButton *validateButton = new QPushButton("Valider", dialog);
    connect(validateButton, &QPushButton::clicked, [=]() {
        QString nom = nomCombo->currentText();
        QString question = questionCombo->currentText();
        QString reponse = answerInput->text().trimmed();
        QString newPwd = newPasswordInput->text().trimmed();

        if(nom.isEmpty() || reponse.isEmpty() || newPwd.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Tous les champs sont obligatoires");
            return;
        }

        if (verifySecretAnswer(nom, question, reponse)) {
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE EMPLOYEE SET PASSWORD = :pwd WHERE NOM = :nom");
            updateQuery.bindValue(":pwd", newPwd);
            updateQuery.bindValue(":nom", nom);

            if (updateQuery.exec()) {
                QMessageBox::information(dialog, "Succès", "Mot de passe mis à jour !");
                dialog->close();
            } else {
                QMessageBox::critical(dialog, "Erreur", "Échec de la mise à jour: " + updateQuery.lastError().text());
            }
        } else {
            QMessageBox::warning(dialog, "Erreur", "Réponse incorrecte ou employé inconnu");
        }
    });

    layout->addRow(validateButton);
    dialog->exec();
}

bool MainWindow::verifySecretAnswer(const QString &nom, const QString &question, const QString &reponse) {
    QSqlQuery query;
    query.prepare("SELECT REPONSE FROM EMPLOYEE WHERE NOM = :nom AND QUESTION = :question");
    query.bindValue(":nom", nom);
    query.bindValue(":question", question);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString reponseStockee = query.value(0).toString().trimmed();
        //QString reponseHash = QCryptographicHash::hash(reponse.trimmed().toUtf8(), QCryptographicHash::Sha256).toHex();

        qDebug() << "Nom:" << nom;
        qDebug() << "Question:" << question;
        qDebug() << "Réponse fournie:" << reponse;
        qDebug() << "Hash stocké:" << reponseStockee;
       // qDebug() << "Hash généré:" << reponseHash;

       // return (reponseHash == reponseStockee);
    } else {
        qDebug() << "Aucun résultat trouvé pour nom:" << nom << "et question:" << question;
        return false;
    }
}
void MainWindow::onResetPasswordClicked()
{
    // Implémentation si vous avez un bouton spécifique pour la réinitialisation
    // Sinon, cette méthode peut être supprimée de vos déclarations
}
void MainWindow::onShowPasswordToggled(bool checked) {
    ui->pwd->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}


void MainWindow::enregistrerHistorique(const QString &utilisateur, const QString &action, const QString &cible)
{
    QSqlQuery query;
    query.prepare("INSERT INTO HISTORIQUE (NOM_UTILISATEUR, ACTION, CIBLE, DATE_ACTION, HEURE_ACTION) "
                  "VALUES (:utilisateur, :action, :cible, :date, :heure)");
    query.bindValue(":utilisateur", utilisateur);
    query.bindValue(":action", action);
    query.bindValue(":cible", cible);
    query.bindValue(":date", QDate::currentDate().toString("dd/MM/yyyy"));
    query.bindValue(":heure", QTime::currentTime().toString("hh:mm:ss"));
    query.exec();
}

