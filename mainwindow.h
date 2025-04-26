#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "evenements.h"
#include <QMainWindow>

#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
<<<<<<< HEAD
#include "arduino.h"

=======
>>>>>>> 983537a7dd52ff7ff4e5d09ae92c7113c9018da9
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
    void onSupprimerClicked();
    void on_pushButton_export_pdf_clicked();
    void on_pushbutton_ajouter_2_clicked();
    void on_comboBox_2_currentIndexChanged(int index);
    void on_pushButton_180_clicked();
    void showNotification();

    void on_pushButton_modifier_clicked();

    //void on_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_stats_clicked();
    void on_pushButton_22_clicked();
    void on_sortComboBox_changed();
    void on_tri_clicked();
    void on_pushButton_search_clicked();  // New slot for search button
    void on_lineEdit_search_textChanged(const QString &text);
    void on_pushButton_historique_clicked();
    void afficherEvenements(); // Affichage normal
    void onComboBox2IndexChanged(int index);
    void checkUpcomingEvents();
    void showNotification(const QString &message);

<<<<<<< HEAD

    void on_pushButton_testarduino_clicked();

    void on_pushButton_arduinoff_clicked();
    void update_label();
=======
>>>>>>> 983537a7dd52ff7ff4e5d09ae92c7113c9018da9

private:
    Ui::MainWindow *ui;
    evenement e;
     QSystemTrayIcon *trayIcon;
<<<<<<< HEAD
    QByteArray data; // variable contenant les données reçues

    Arduino A; // objet temporaire
=======
>>>>>>> 983537a7dd52ff7ff4e5d09ae92c7113c9018da9
};
#endif // MAINWINDOW_H
