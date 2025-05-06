#include "login.h"
#include "ui_login.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>


LOGIN::~LOGIN()
{
    delete ui;
}
LOGIN::LOGIN(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LOGIN)
{
    ui->setupUi(this);  // Doit exister !

    // ✅ Créer le bouton
    QPushButton *forgotPasswordButton = new QPushButton("Mot de passe oublié ?", this);

    // ✅ Connecter le signal au slot
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LOGIN::on_btnForgotPassword_clicked);
    connect(ui->showPasswordCheckbox, &QCheckBox::toggled, this, &LOGIN::onShowPasswordToggled);


    // ✅ Ajouter au layout
    if (ui->btnForgotPassword) {
        ui->btnForgotPassword->addWidget(forgotPasswordButton);
    } else {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(forgotPasswordButton);
        setLayout(layout);
    }
}

void LOGIN::on_btnLogin_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString mail = ui->mail->text();
    QString password = ui->lineEdit_password->text();

    QSqlQuery query;

    // Vérifie si le nom existe
    query.prepare("SELECT EMAIL, PASSWORD FROM EMPLOYEE WHERE NOM = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Erreur lors de l'accès à la base de données.");
        return;
    }
    // Vérification du format de l'email
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
    if (!emailRegex.match(mail).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Adresse email invalide !");
        return;
    }

    // Vérification de la complexité du mot de passe (au moins 6 caractères avec une lettre et un chiffre)
    QRegularExpression pwdRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
    if (!pwdRegex.match(password).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 6 caractères, avec au moins une lettre et un chiffre !");
        return;
    }

    if (query.next()) {
        QString storedMail = query.value("EMAIL").toString();
        QString storedPassword = query.value("PASSWORD").toString();

        if (storedMail != mail) {
            QMessageBox::warning(this, "Erreur", "Email incorrect pour ce nom d'utilisateur.");
        } else if (storedPassword != password) {
            QMessageBox::warning(this, "Erreur", "Mot de passe incorrect.");
        } else {
            // Tout est bon
            mainWindow = new MainWindow();
            mainWindow->show();
            this->close();
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Nom d'utilisateur incorrect.");
    }
}
void LOGIN::onShowPasswordToggled(bool checked) {
    ui->lineEdit_password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}
void LOGIN::on_btnCreateAccount_clicked()
{
        mainWindow = new MainWindow();  // Open MainWindow
        mainWindow->show();
        this->close();  // Close the login window
}


bool LOGIN::verifySecretAnswer(const QString &nom, const QString &question, const QString &reponse) {
    QSqlQuery query;
    query.prepare("SELECT REPONSE FROM EMPLOYEE WHERE NOM = :nom AND QUESTION = :question");
    query.bindValue(":nom", nom);
    query.bindValue(":question", question);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString reponseStockee = query.value(0).toString().trimmed();
        return (reponseStockee == reponse);
    } else {
        return false;
    }
}
void LOGIN::updatePassword(const QString &nom, const QString &newPassword) {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET PASSWORD = :pwd WHERE NOM = :nom");
    query.bindValue(":pwd", newPassword);
    query.bindValue(":nom", nom);
    // Vérification de la complexité du mot de passe (au moins 6 caractères avec une lettre et un chiffre)
    QRegularExpression pwdRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
    if (!pwdRegex.match(newPassword).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 6 caractères, avec au moins une lettre et un chiffre !");
        return;
    }
    query.exec();
}
void LOGIN::showPasswordResetDialog() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Réinitialisation du mot de passe");

    QFormLayout *layout = new QFormLayout(dialog);

    QComboBox *nomCombo = new QComboBox(dialog);
    QSqlQuery query("SELECT DISTINCT NOM FROM EMPLOYEE");
    while (query.next()) {
        nomCombo->addItem(query.value(0).toString());
    }
    layout->addRow("Nom :", nomCombo);

    QComboBox *questionCombo = new QComboBox(dialog);
    questionCombo->addItems({
        "Quel est le nom de votre premier animal ?",
        "Quelle est votre ville de naissance ?",
        "Quel était le nom de votre premier professeur ?"
    });
    layout->addRow("Question :", questionCombo);

    QLineEdit *answerInput = new QLineEdit(dialog);
    layout->addRow("Réponse :", answerInput);

    QLineEdit *newPasswordInput = new QLineEdit(dialog);
    newPasswordInput->setEchoMode(QLineEdit::Password);
    layout->addRow("Nouveau mot de passe :", newPasswordInput);

    QPushButton *validateButton = new QPushButton("Valider", dialog);
    connect(validateButton, &QPushButton::clicked, [=]() {
        QString nom = nomCombo->currentText();
        QString question = questionCombo->currentText();
        QString reponse = answerInput->text().trimmed();
        QString newPwd = newPasswordInput->text().trimmed();

        if (nom.isEmpty() || reponse.isEmpty() || newPwd.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Tous les champs sont obligatoires");
            return;
        }


        if (verifySecretAnswer(nom, question, reponse)) {
            updatePassword(nom, newPwd);
            // Vérification de la complexité du mot de passe (au moins 6 caractères avec une lettre et un chiffre)
            QRegularExpression pwdRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
            if (!pwdRegex.match(newPwd).hasMatch()) {
                QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 6 caractères, avec au moins une lettre et un chiffre !");
                return;
            }
            QMessageBox::information(dialog, "Succès", "Mot de passe mis à jour !");
            dialog->close();
        } else {
            QMessageBox::warning(dialog, "Erreur", "Réponse incorrecte ");
        }
    });
    layout->addRow(validateButton);

    dialog->exec();
}
void LOGIN::on_btnForgotPassword_clicked()
{
    showPasswordResetDialog();
}
