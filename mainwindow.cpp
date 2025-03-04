#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include "evenements.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //int i;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_pushbutton_ajouter_clicked()
{
    evenement e;
    e.Setnom(ui->lineedit_nom->text());
    e.Setdate_debut(ui->lineedit_date_debut->date());
    e.Setdate_fin(ui->lineedit_date_fin->date());

}*/

