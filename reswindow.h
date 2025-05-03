#ifndef RESWINDOW_H
#define RESWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "ui_mainwindow.h"

class ResWindow : public QMainWindow
{
    Q_OBJECT

public:
    ResWindow(QWidget *parent = nullptr);
    ~ResWindow();

private slots:
    void ajouterRessource();
    void modifierRessource();
    void supprimerRessource();
    void afficherStatistiquesCamembert();
    void exporterPDF();
    void TrierParDisponibilite();
    void filtrerRessources();
    void afficherDatesMaintenance();
    void afficherRessourcesPourDate(const QDate &date);
    void on_pushButton_qr_clicked();
    void readFromArduino();
    void traiterSurtension(int id);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    QString buffer;
    void afficherRessources();
    void genererQRCode(const QString &texte, const QString &nomFichier);
};

#endif // RESWINDOW_H
