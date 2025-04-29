#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:



private slots:
    void afficherRessources();
    void ajouterRessource();
    void modifierRessource();
    void supprimerRessource();
    void exporterPDF();
    void TrierParDisponibilite();
    void on_comboBox_tri_currentTextChanged(const QString &etat);
    void afficherDatesMaintenance();
    void on_pushButton_qr_clicked();
    void appliquerTri();
    void trierTableParColonne(int colonne, Qt::SortOrder ordre);
    void afficherStatistiquesCamembert();
    void readFromArduino();


private:
    Ui::MainWindow *ui;
    void genererQRCode(const QString& texte, const QString& nomFichier);
    void afficherRessourcesPourDate(const QDate &date);
    QSerialPort *arduino;
    QString buffer;

    void traiterSurtension(int id);

};

#endif // MAINWINDOW_H
