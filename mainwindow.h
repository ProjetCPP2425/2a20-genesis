#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "evenements.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QPrinter>
#include <QPainter>
#include <QFileDialog>


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

    void afficherRessources();
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

  //  void on_pushButton_arduinoff_clicked();
    //void update_label();

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

};
#endif // MAINWINDOW_H
