#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define LOGIN_H

#include "evenements.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QPrinter>
#include <QPainter>
#include <QFileDialog>

#include <QSqlError>
#include <QSqlQueryModel>
#include "locataire.h"
#include "qtexttospeech.h"
#include "boutique.h"

#include <QMessageBox>
#include <QDate>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QtCharts>    // Modifié pour inclure tous les composants charts
#include <QSqlRecord>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshTable();
    void afficherRessources();

private slots:
    void onRowSelected();
    //void onSupprimerClicked();
    void on_pushButton_export_pdf_3_clicked();
    void on_pushbutton_ajouter_clicked();
    //void on_comboBox_2_currentIndexChanged(int index);
    //void on_pushButton_180_clicked();
    //void showNotification();

    void on_pushButton_modifier_3_clicked();

    //void on_modifier_clicked();
    void on_pushButton_supprimer_3_clicked();
    void on_pushButton_stats_3_clicked();
    //void on_pushButton_22_clicked();
    //void on_sortComboBox_changed();
    //void on_tri_clicked();
    void on_pushButton_search_clicked();  // New slot for search button
    void on_lineEdit_search_3_textChanged(const QString &text);
    void on_pushButton_historique_3_clicked();
    //void afficherEvenements(); // Affichage normal
    void onComboBox2IndexChanged(int index);
    void checkUpcomingEvents();
    void showNotification(const QString &message);

    //void afficherRessources();

    void ajouterRessource();
    void modifierRessource();
    void supprimerRessource();
    void exporterPDF();
    void TrierParDisponibilite();
    void afficherDatesMaintenance();
    void on_pushButton_qr_3_clicked();
    //void appliquerTri();
    //void trierTableParColonne(int colonne, Qt::SortOrder ordre);
    void afficherStatistiquesCamembert();
    void readFromArduino();
    void filtrerRessources();

    //void on_pushButton_testarduino_clicked();

    //void on_pushButton_arduinoff_clicked();
    //void update_label();

    //////////////BOUTIQUES////////////
    void on_ajouter_clicked();   // for the "ajouter" button
    void on_supprimer_clicked(); // for the "supprimer" button
    void on_modifier_clicked();  // only if you have a button named "modifier"
    void on_lire_clicked();
    void on_pdf_clicked();
    void on_chercher_clicked();
    void on_tri_clicked();
    void on_statistique_clicked();
    void afficherCarteInteractive();

    /////////////LOCATAIRES////////////
    void on_ajtr_clicked();
    void on_dlt_clicked();
    void on_mdf_clicked();
    void on_trr_clicked();
    void on_rchb_clicked();
    void on_pdfl_clicked();
    void on_stts_clicked();
    void on_mlng_clicked();
    void on_vn_clicked();
    void sendSMS_Infobip(const QString& recipientPhone);
    void readFromArduinol();
    bool checkRFIDInDatabase(const QString& cardID);
    void fillTableFromModell(QSqlQueryModel *modell);

    /////////////EMPLOYES/////////////
    void onTriClicked();
    void onvaliderClicked();
    void onvalClicked();
    void ondelClicked();
    void onupClicked();
    void populateFields(int row);
    void trierEmployes(const QString &critere);
    void rechercherEmploye(const QString &criteres);
    void onSearchClicked();
    void searchEmployes(const QString &searchText);
    void onpdfclicked();
    void on_statbutton_clicked();
    void onDashboardClicked();

    void onPasswordForgotClicked();  // Bouton "Mot de passe oublié"
    void onResetPasswordClicked();   // Validation de la réinitialisation
    void onShowPasswordToggled(bool checked);

    void onSerialDataReceived();

    void onHistoriqueClicked();

////

    void on_gesl_clicked();
    void on_gesb_clicked();
    void on_gesev_clicked();
    void on_gesr_clicked();
    void on_geslis_clicked();
    void on_gesbis_clicked();
    void on_gesevis_clicked();
    void on_gesris_clicked();
    void on_gesley_clicked();
    void on_gesbey_clicked();
    void on_gesevey_clicked();
    void on_gesrey_clicked();
    void on_geslim_clicked();
    void on_gesbim_clicked();
    void on_gesevim_clicked();
    void on_gesrim_clicked();



private:
    Ui::MainWindow *ui;
    evenement e;
     QSystemTrayIcon *trayIcon;
    QByteArray data; // variable contenant les données reçues

     void genererQRCode(const QString& texte, const QString& nomFichier);
     void afficherRessourcesPourDate(const QDate &date);
     QSerialPort *arduino;
     QString buffer;

     void traiterSurtension(int id);
     QTextToSpeech *speech;
     // Helper to fill the QTableWidget from a QSqlQueryModel
     void fillTableFromModel(QSqlQueryModel *model);
     QPixmap *pixmap;
     locataires L;
     QString arduinoData;

     //EMP
     void refreshTableWithQuery(QSqlQuery &query);
     void addPieChartToTab(QTabWidget *tabWidget, const QString &title, QPieSeries *series, const QStringList &colors);
     void addBarChartToTab(QTabWidget *tabWidget, const QString &title, QBarSeries *series, const QStringList &colors);
     int calculateEmployeesInRange(int minAge, int maxAge);

     QVBoxLayout *verticalLayout;

     void showPasswordResetDialog();
     bool verifySecretAnswer(const QString &nom, const QString &question, const QString &reponse);
     void updatePassword(const QString &nom, const QString &newPassword);
     QStringList getSecretQuestions(); // Récupère toutes les questions disponibles
     void enregistrerHistorique(const QString &utilisateur, const QString &action, const QString &cible);

     void val();

};
#endif // MAINWINDOW_H
