#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define LOGIN_H


#include <QMainWindow>
#include <QTableWidgetItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QtCharts>    // Modifié pour inclure tous les composants charts
#include <QSqlRecord>  // Ajouté pour QSqlRecord
#include <QSerialPort>
#include <QSerialPortInfo>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshTable();
private:
    void refreshTableWithQuery(QSqlQuery &query);
    void addPieChartToTab(QTabWidget *tabWidget, const QString &title, QPieSeries *series, const QStringList &colors);
    void addBarChartToTab(QTabWidget *tabWidget, const QString &title, QBarSeries *series, const QStringList &colors);    int calculateEmployeesInRange(int minAge, int maxAge);

private slots:
    void onTriClicked();
    void onvaliderClicked();
    void onvalClicked();
    void ondelClicked();
    void onupClicked();
    void populateFields(int row);
    void trierEmployes(const QString &critere);
    void rechercherEmploye(const QString &criteres);
    void onSearchClicked();
    void searchEmployes(const QString &searchText);
    void onpdfclicked();
    void on_statbutton_clicked();
    void onDashboardClicked();  // Déclaration
private slots:
    void onPasswordForgotClicked();  // Bouton "Mot de passe oublié"
    void onResetPasswordClicked();   // Validation de la réinitialisation
    void onShowPasswordToggled(bool checked);

private slots:
    void onSerialDataReceived();


private:
    QSerialPort *serial;

private slots:
    void onHistoriqueClicked();

private:
    QVBoxLayout *verticalLayout;  // Déclarez le layout comme membre de classe
private:
    void showPasswordResetDialog();
    bool verifySecretAnswer(const QString &nom, const QString &question, const QString &reponse);
    void updatePassword(const QString &nom, const QString &newPassword);
    QStringList getSecretQuestions(); // Récupère toutes les questions disponibles
    void enregistrerHistorique(const QString &utilisateur, const QString &action, const QString &cible);

private:
    Ui::MainWindow *ui;

    void val();
};



#endif // MAINWINDOW_H
