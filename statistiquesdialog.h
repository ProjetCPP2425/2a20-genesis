#ifndef STATISTIQUESDIALOG_H
#define STATISTIQUESDIALOG_H

#include <QDialog>

namespace Ui {
class StatistiquesDialog;
}

class StatistiquesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatistiquesDialog(QWidget *parent = nullptr);
    ~StatistiquesDialog();
    void afficherStatistiques();  // Méthode pour afficher les statistiques

private:
    Ui::StatistiquesDialog *ui;  // Lien vers l'interface graphique
};

#endif // STATISTIQUESDIALOG_H
