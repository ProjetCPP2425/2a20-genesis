#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtexttospeech.h"
#include <QMainWindow>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_clicked();   // for the "ajouter" button
    void on_supprimer_clicked(); // for the "supprimer" button
    void on_modifier_clicked();  // only if you have a button named "modifier"
    void on_lire_clicked();
    void on_pdf_clicked();
    void on_chercher_clicked();
    void on_tri_clicked();
    void on_statistique_clicked();
    void afficherCarteInteractive();
private:
    Ui::MainWindow *ui;

    // ✅ Add this to avoid multiple declaration errors
    QTextToSpeech *speech;
    // Helper to fill the QTableWidget from a QSqlQueryModel
    void fillTableFromModel(QSqlQueryModel *model);
};

#endif // MAINWINDOW_H
