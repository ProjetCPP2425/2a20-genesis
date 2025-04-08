/********************************************************************************
** Form generated from reading UI file 'statistiquesdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTIQUESDIALOG_H
#define UI_STATISTIQUESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_StatistiquesDialog
{
public:
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QLabel *label_localisation;
    QLabel *label_prix;

    void setupUi(QDialog *StatistiquesDialog)
    {
        if (StatistiquesDialog->objectName().isEmpty())
            StatistiquesDialog->setObjectName("StatistiquesDialog");
        StatistiquesDialog->resize(622, 568);
        lineEdit_2 = new QLineEdit(StatistiquesDialog);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(360, 160, 191, 251));
        lineEdit = new QLineEdit(StatistiquesDialog);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(50, 160, 191, 251));
        label_localisation = new QLabel(StatistiquesDialog);
        label_localisation->setObjectName("label_localisation");
        label_localisation->setGeometry(QRect(60, 100, 201, 31));
        label_prix = new QLabel(StatistiquesDialog);
        label_prix->setObjectName("label_prix");
        label_prix->setGeometry(QRect(350, 100, 221, 31));

        retranslateUi(StatistiquesDialog);

        QMetaObject::connectSlotsByName(StatistiquesDialog);
    } // setupUi

    void retranslateUi(QDialog *StatistiquesDialog)
    {
        StatistiquesDialog->setWindowTitle(QCoreApplication::translate("StatistiquesDialog", "Statistiques des ressources", nullptr));
        label_localisation->setText(QCoreApplication::translate("StatistiquesDialog", "Ressources utilisees par bloc", nullptr));
        label_prix->setText(QCoreApplication::translate("StatistiquesDialog", "Prix moyen de chaque ressource", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatistiquesDialog: public Ui_StatistiquesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTIQUESDIALOG_H
