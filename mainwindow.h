#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

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
    void ouvrirStatistiques();



private slots:
    void afficherRessources();
    void ajouterRessource();
    void modifierRessource();
    void supprimerRessource();
    void calculerStatistiques();
    void exporterPDF();
    void TrierParDisponibilite();
    void on_comboBox_tri_currentTextChanged(const QString &etat);
    void afficherDatesMaintenance();

private:
    Ui::MainWindow *ui;
    void genererQRCode(const QString& texte, const QString& nomFichier);
    void afficherRessourcesPourDate(const QDate &date);

};

#endif // MAINWINDOW_H
