#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include "locataire.h"

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
    //void on_pushButton_ajtr_clicked();
   // void on_pushButton_supprimer_clicked();

    void fillTableFromModel(QSqlQueryModel *model);
    void on_ajtr_clicked();
    void on_dlt_clicked();
    void on_mdf_clicked();
    /*void on_pdf_clicked();
    void on_alerte_clicked();*/

private:
    Ui::MainWindow *ui;
    QPixmap *pixmap;

    locataires L;
};

#endif // MAINWINDOW_H
