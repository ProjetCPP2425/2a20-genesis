/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_valider;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_9;
    QDateEdit *dateEdit_3;
    QDateEdit *dateEdit_4;
    QLabel *label_15;
    QLineEdit *lineEdit_11;
    QLabel *label_9;
    QComboBox *comboBox_type;
    QComboBox *comboBox_etat;
    QGroupBox *groupBox;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QLabel *label_17;
    QGroupBox *groupBox_3;
    QTableWidget *tableWidget;
    QCalendarWidget *calendar_maintenance;
    QPushButton *pushButton_stats;
    QPushButton *pushButton_pdf;
    QPushButton *pushButton_supp;
    QPushButton *pushButton_modif;
    QComboBox *comboBox_tri;
    QPushButton *pushButton_tri;
    QPushButton *pushButton_qr;
    QComboBox *comboBox_Tri;
    QPushButton *pushButton_Tri;
    QLabel *label;
    QStatusBar *statusbar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1749, 962);
        MainWindow->setStyleSheet(QString::fromUtf8("/* \360\237\214\237 Global Styles */\n"
"* {\n"
"    font-family: Arial, sans-serif;\n"
"    color: #213555; /* Dark blue for text */\n"
"}\n"
"\n"
"/* \360\237\214\237 Background - Solid Beige */\n"
"QWidget {\n"
"    background-color: #f5efe7; /* Beige */\n"
"}\n"
"\n"
"/* \360\237\214\237 Group Box - Forms & Sections */\n"
"QGroupBox {\n"
"    background-color: #f5efe7;\n"
"    border: 2px solid #3e5879;\n"
"    border-radius: 10px;\n"
"    margin-top: 20px;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    background-color: #3e5879;\n"
"    color: white;\n"
"    padding: 6px 12px;\n"
"    border-radius: 5px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* \360\237\214\237 Input Fields */\n"
"QLineEdit,\n"
"QTextEdit,\n"
"QComboBox,\n"
"QDateEdit,\n"
"QTimeEdit {\n"
"    border: 2px solid #3e5879;\n"
"    border-radius: 8px;\n"
"    padding: 6px;\n"
"    font-size: 14px;\n"
"    background-color:"
                        " #ffffff;\n"
"}\n"
"\n"
"QLineEdit:focus,\n"
"QTextEdit:focus,\n"
"QComboBox:focus,\n"
"QDateEdit:focus,\n"
"QTimeEdit:focus {\n"
"    border: 2px solid #213555;\n"
"}\n"
"\n"
"/* \360\237\214\237 Buttons */\n"
"QPushButton {\n"
"    background-color: #3e5879;\n"
"    color: #ffffff;\n"
"    border-radius: 12px;\n"
"    padding: 10px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    border: 2px solid #213555;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #f5efe7;\n"
"    color: #213555;\n"
"    border: 2px solid #3e5879;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #213555;\n"
"    color: white;\n"
"    border: 2px solid #3e5879;\n"
"}\n"
"\n"
"/* \360\237\214\237 Action Buttons */\n"
"QPushButton#edit {\n"
"    background-color: #3e5879;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}\n"
"\n"
"QPushButton#delete {\n"
"    background-color: #d9534f;\n"
"    color: white;\n"
"    border-radiu"
                        "s: 6px;\n"
"    padding: 6px;\n"
"}\n"
"\n"
"QPushButton#delete:hover {\n"
"    background-color: #c9302c;\n"
"}\n"
"\n"
"/* \360\237\214\237 Tables */\n"
"QTableWidget {\n"
"    border: 2px solid #3e5879;\n"
"    background-color: #f5efe7;\n"
"    gridline-color: #3e5879;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #213555;\n"
"    color: white;\n"
"    padding: 7px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QTableWidget::item:hover {\n"
"    background-color: #e0d6cd;\n"
"}\n"
"\n"
"/* \360\237\214\237 Sidebar */\n"
"QFrame#sidebar {\n"
"    background-color: #213555;\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QListWidget#menu {\n"
"    background: transparent;\n"
"    color: #f5efe7;\n"
"    font-size: 14px;\n"
"    border: none;\n"
"}\n"
"\n"
"QListWidget#menu::item:hover {\n"
"    background-color: #3e5879;\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"/* \360\237\214\237 S"
                        "earch Bar */\n"
"QLineEdit#searchBar {\n"
"    border: 2px solid #3e5879;\n"
"    border-radius: 8px;\n"
"    padding: 6px;\n"
"    font-size: 14px;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QLineEdit#searchBar:focus {\n"
"    border: 2px solid #213555;\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(270, 10, 291, 771));
        pushButton_valider = new QPushButton(groupBox_2);
        pushButton_valider->setObjectName("pushButton_valider");
        pushButton_valider->setGeometry(QRect(90, 680, 111, 41));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(80, 140, 31, 20));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(60, 310, 63, 20));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(70, 390, 31, 20));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(50, 470, 81, 20));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(50, 540, 71, 20));
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(10, 610, 131, 21));
        lineEdit_7 = new QLineEdit(groupBox_2);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(140, 300, 131, 31));
        lineEdit_8 = new QLineEdit(groupBox_2);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(140, 380, 131, 31));
        lineEdit_9 = new QLineEdit(groupBox_2);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(140, 460, 131, 31));
        dateEdit_3 = new QDateEdit(groupBox_2);
        dateEdit_3->setObjectName("dateEdit_3");
        dateEdit_3->setGeometry(QRect(140, 540, 131, 31));
        dateEdit_4 = new QDateEdit(groupBox_2);
        dateEdit_4->setObjectName("dateEdit_4");
        dateEdit_4->setGeometry(QRect(140, 610, 131, 31));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(80, 60, 41, 20));
        lineEdit_11 = new QLineEdit(groupBox_2);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(140, 70, 131, 31));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(80, 230, 31, 20));
        comboBox_type = new QComboBox(groupBox_2);
        comboBox_type->setObjectName("comboBox_type");
        comboBox_type->setGeometry(QRect(140, 140, 131, 31));
        comboBox_etat = new QComboBox(groupBox_2);
        comboBox_etat->setObjectName("comboBox_etat");
        comboBox_etat->setGeometry(QRect(140, 220, 131, 31));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 10, 261, 771));
        pushButton_12 = new QPushButton(groupBox);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(30, 180, 211, 41));
        pushButton_13 = new QPushButton(groupBox);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(40, 240, 181, 41));
        pushButton_14 = new QPushButton(groupBox);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(40, 300, 181, 41));
        pushButton_15 = new QPushButton(groupBox);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(40, 360, 181, 41));
        pushButton_16 = new QPushButton(groupBox);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(40, 420, 181, 41));
        label_17 = new QLabel(groupBox);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(90, 40, 101, 91));
        label_17->setPixmap(QPixmap(QString::fromUtf8("lowgo.png")));
        label_17->setScaledContents(true);
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(580, 10, 951, 771));
        tableWidget = new QTableWidget(groupBox_3);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (tableWidget->rowCount() < 3)
            tableWidget->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setItem(0, 3, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget->setItem(0, 4, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget->setItem(0, 5, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget->setItem(0, 6, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget->setItem(1, 0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget->setItem(1, 1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget->setItem(1, 2, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget->setItem(1, 3, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget->setItem(1, 4, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget->setItem(1, 5, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget->setItem(1, 6, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget->setItem(2, 0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget->setItem(2, 1, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableWidget->setItem(2, 2, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tableWidget->setItem(2, 3, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tableWidget->setItem(2, 4, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        tableWidget->setItem(2, 5, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        tableWidget->setItem(2, 6, __qtablewidgetitem30);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(30, 90, 901, 301));
        calendar_maintenance = new QCalendarWidget(groupBox_3);
        calendar_maintenance->setObjectName("calendar_maintenance");
        calendar_maintenance->setGeometry(QRect(30, 450, 901, 261));
        pushButton_stats = new QPushButton(groupBox_3);
        pushButton_stats->setObjectName("pushButton_stats");
        pushButton_stats->setGeometry(QRect(800, 400, 131, 41));
        pushButton_pdf = new QPushButton(groupBox_3);
        pushButton_pdf->setObjectName("pushButton_pdf");
        pushButton_pdf->setGeometry(QRect(490, 400, 151, 41));
        pushButton_supp = new QPushButton(groupBox_3);
        pushButton_supp->setObjectName("pushButton_supp");
        pushButton_supp->setGeometry(QRect(720, 400, 61, 41));
        pushButton_modif = new QPushButton(groupBox_3);
        pushButton_modif->setObjectName("pushButton_modif");
        pushButton_modif->setGeometry(QRect(650, 400, 61, 41));
        comboBox_tri = new QComboBox(groupBox_3);
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->setObjectName("comboBox_tri");
        comboBox_tri->setGeometry(QRect(690, 40, 221, 31));
        pushButton_tri = new QPushButton(groupBox_3);
        pushButton_tri->setObjectName("pushButton_tri");
        pushButton_tri->setGeometry(QRect(550, 40, 121, 41));
        pushButton_qr = new QPushButton(groupBox_3);
        pushButton_qr->setObjectName("pushButton_qr");
        pushButton_qr->setGeometry(QRect(350, 400, 121, 41));
        comboBox_Tri = new QComboBox(groupBox_3);
        comboBox_Tri->setObjectName("comboBox_Tri");
        comboBox_Tri->setGeometry(QRect(360, 40, 171, 31));
        pushButton_Tri = new QPushButton(groupBox_3);
        pushButton_Tri->setObjectName("pushButton_Tri");
        pushButton_Tri->setGeometry(QRect(220, 40, 121, 41));
        label = new QLabel(groupBox_3);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 420, 321, 20));
        MainWindow->setCentralWidget(centralwidget);
        groupBox_3->raise();
        groupBox_2->raise();
        groupBox->raise();
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1749, 23));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Ajouter Ressource", nullptr));
        pushButton_valider->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Localisation", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "date achat ", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Date maintenance", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Gestion Ressources", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Gestion Ressources", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Gestion Boutiques ", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "Gestion \303\251v\303\250nement", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Gestion locataires", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "Gestion Employ\303\251s", nullptr));
        label_17->setText(QString());
        groupBox_3->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Localisation", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Date Achat", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        tableWidget->setSortingEnabled(__sortingEnabled);

        pushButton_stats->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        pushButton_pdf->setText(QCoreApplication::translate("MainWindow", "Exporter PDF", nullptr));
        pushButton_supp->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217", nullptr));
        pushButton_modif->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        comboBox_tri->setItemText(0, QCoreApplication::translate("MainWindow", "Type", nullptr));
        comboBox_tri->setItemText(1, QCoreApplication::translate("MainWindow", "Prix", nullptr));
        comboBox_tri->setItemText(2, QCoreApplication::translate("MainWindow", "Localisation", nullptr));

        pushButton_tri->setText(QCoreApplication::translate("MainWindow", "recherche", nullptr));
        pushButton_qr->setText(QCoreApplication::translate("MainWindow", "qrcode", nullptr));
        pushButton_Tri->setText(QCoreApplication::translate("MainWindow", "Tri", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
