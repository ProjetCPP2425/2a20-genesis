#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "evenements.h"
#include <QMainWindow>

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
    void on_pushbutton_ajouter_2_clicked();

    void on_pushButton_180_clicked();


    void on_pushButton_modifier_clicked();

    //void on_modifier_clicked();
    void on_pushButton_supprimer_clicked();

private:
    Ui::MainWindow *ui;
    evenement e;
};
#endif // MAINWINDOW_H
