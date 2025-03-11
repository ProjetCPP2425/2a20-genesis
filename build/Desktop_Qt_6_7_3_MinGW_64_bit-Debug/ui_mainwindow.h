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
    QGroupBox *groupBox;
    QPushButton *ajtr;
    QLabel *label_8;
    QComboBox *mp;
    QLineEdit *mail;
    QLabel *label_4;
    QDateEdit *dins;
    QDateEdit *dfc;
    QComboBox *st;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_9;
    QLineEdit *px;
    QLineEdit *nom;
    QLabel *label;
    QGroupBox *groupBox_2;
    QLabel *label_7;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QGroupBox *groupBox_3;
    QPushButton *pushButton_3;
    QPushButton *stts;
    QPushButton *mdf;
    QPushButton *dlt;
    QPushButton *trr;
    QComboBox *trc;
    QPushButton *alerte;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(1204, 686);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("/* \360\237\214\237 Global Styles */\n"
"* {\n"
"    font-family: Arial, sans-serif;\n"
"    color: #213555; /* Dark blue for text */\n"
"}\n"
"\n"
"/* \360\237\214\237 Background - Soft Beige Gradient */\n"
"QWidget {\n"
"    background: qlineargradient(\n"
"        spread: pad,\n"
"        x1: 0, y1: 0,\n"
"        x2: 1, y2: 1,\n"
"        stop: 0 #f5efe7,\n"
"        stop: 1 #e6e0d7\n"
"    );\n"
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
"QTim"
                        "eEdit {\n"
"    border: 2px solid #3e5879;\n"
"    border-radius: 8px;\n"
"    padding: 6px;\n"
"    font-size: 14px;\n"
"    background-color: #ffffff;\n"
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
"    border-radiu"
                        "s: 6px;\n"
"    padding: 6px;\n"
"}\n"
"\n"
"QPushButton#delete {\n"
"    background-color: #d9534f;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
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
"    background-color: #d6cec6;\n"
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
""
                        "    background-color: #3e5879;\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"/* \360\237\214\237 Search Bar */\n"
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
"}/* \360\237\214\237 Global Styles */\n"
"* {\n"
"    font-family: Arial, sans-serif;\n"
"    color: #213555; /* Dark blue for text */\n"
"}\n"
"\n"
"/* \360\237\214\237 Background - Soft Beige Gradient */\n"
"QWidget {\n"
"    background: qlineargradient(\n"
"        spread: pad,\n"
"        x1: 0, y1: 0,\n"
"        x2: 1, y2: 1,\n"
"        stop: 0 #f5efe7,\n"
"        stop: 1 #e6e0d7\n"
"    );\n"
"}\n"
"\n"
"/* \360\237\214\237 Group Box - Forms & Sections */\n"
"QGroupBox {\n"
"    background-color: #f5efe7;\n"
"    border: 2px solid #3e5879;\n"
"    border-radius: 10px;\n"
"    margin-top: 20px;\n"
"    pa"
                        "dding: 10px;\n"
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
"    background-color: #ffffff;\n"
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
"}"
                        "\n"
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
"    border-radius: 6px;\n"
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
""
                        "\n"
"QTableWidget::item:hover {\n"
"    background-color: #d6cec6;\n"
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
"/* \360\237\214\237 Search Bar */\n"
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
"}"));
        MainWindow->setAnimated(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(290, 30, 271, 581));
        ajtr = new QPushButton(groupBox);
        ajtr->setObjectName("ajtr");
        ajtr->setGeometry(QRect(90, 510, 98, 51));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 350, 81, 31));
        mp = new QComboBox(groupBox);
        mp->addItem(QString());
        mp->addItem(QString());
        mp->addItem(QString());
        mp->setObjectName("mp");
        mp->setGeometry(QRect(130, 350, 121, 30));
        mail = new QLineEdit(groupBox);
        mail->setObjectName("mail");
        mail->setGeometry(QRect(130, 110, 121, 30));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 110, 81, 31));
        label_4->setAcceptDrops(false);
        label_4->setFrameShape(QFrame::Shape::NoFrame);
        label_4->setFrameShadow(QFrame::Shadow::Sunken);
        label_4->setLineWidth(1);
        label_4->setTextFormat(Qt::TextFormat::AutoText);
        label_4->setScaledContents(false);
        label_4->setWordWrap(false);
        label_4->setOpenExternalLinks(false);
        dins = new QDateEdit(groupBox);
        dins->setObjectName("dins");
        dins->setGeometry(QRect(130, 170, 121, 31));
        dfc = new QDateEdit(groupBox);
        dfc->setObjectName("dfc");
        dfc->setGeometry(QRect(130, 230, 121, 31));
        st = new QComboBox(groupBox);
        st->addItem(QString());
        st->addItem(QString());
        st->addItem(QString());
        st->setObjectName("st");
        st->setGeometry(QRect(130, 290, 121, 30));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 170, 81, 31));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(30, 230, 81, 31));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, 290, 81, 31));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(30, 410, 81, 31));
        px = new QLineEdit(groupBox);
        px->setObjectName("px");
        px->setGeometry(QRect(130, 410, 121, 30));
        nom = new QLineEdit(groupBox);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(130, 50, 121, 30));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 50, 81, 31));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 30, 251, 581));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(60, 30, 141, 141));
        label_7->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        label_7->setFrameShape(QFrame::Shape::NoFrame);
        label_7->setFrameShadow(QFrame::Shadow::Plain);
        label_7->setPixmap(QPixmap(QString::fromUtf8("../7.png")));
        label_7->setScaledContents(true);
        label_7->setWordWrap(false);
        label_7->setOpenExternalLinks(false);
        pushButton_7 = new QPushButton(groupBox_2);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(20, 200, 211, 51));
        pushButton_8 = new QPushButton(groupBox_2);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(20, 260, 211, 51));
        pushButton_9 = new QPushButton(groupBox_2);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(20, 320, 211, 51));
        pushButton_10 = new QPushButton(groupBox_2);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(20, 380, 211, 51));
        pushButton_11 = new QPushButton(groupBox_2);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(20, 440, 211, 51));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(580, 30, 601, 581));
        pushButton_3 = new QPushButton(groupBox_3);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(480, 510, 91, 51));
        stts = new QPushButton(groupBox_3);
        stts->setObjectName("stts");
        stts->setGeometry(QRect(150, 40, 131, 51));
        mdf = new QPushButton(groupBox_3);
        mdf->setObjectName("mdf");
        mdf->setGeometry(QRect(250, 510, 51, 51));
        dlt = new QPushButton(groupBox_3);
        dlt->setObjectName("dlt");
        dlt->setGeometry(QRect(320, 510, 51, 51));
        trr = new QPushButton(groupBox_3);
        trr->setObjectName("trr");
        trr->setGeometry(QRect(410, 110, 81, 41));
        trc = new QComboBox(groupBox_3);
        trc->addItem(QString());
        trc->addItem(QString());
        trc->addItem(QString());
        trc->addItem(QString());
        trc->setObjectName("trc");
        trc->setGeometry(QRect(500, 110, 81, 41));
        alerte = new QPushButton(groupBox_3);
        alerte->setObjectName("alerte");
        alerte->setGeometry(QRect(330, 40, 131, 51));
        tableWidget = new QTableWidget(groupBox_3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(20, 160, 561, 341));
        MainWindow->setCentralWidget(centralwidget);
        groupBox_3->raise();
        groupBox->raise();
        groupBox_2->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1204, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QString());
        ajtr->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Mode", nullptr));
        mp->setItemText(0, QCoreApplication::translate("MainWindow", "Espece", nullptr));
        mp->setItemText(1, QCoreApplication::translate("MainWindow", "Cheque", nullptr));
        mp->setItemText(2, QCoreApplication::translate("MainWindow", "Virement", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        st->setItemText(0, QCoreApplication::translate("MainWindow", "Pay\303\251", nullptr));
        st->setItemText(1, QCoreApplication::translate("MainWindow", "Impay\303\251", nullptr));
        st->setItemText(2, QCoreApplication::translate("MainWindow", "En attente", nullptr));

        label_3->setText(QCoreApplication::translate("MainWindow", "Date Inscrit", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Date Fin", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        groupBox_2->setTitle(QString());
        label_7->setText(QString());
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "GESTION BOUTIQUES", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "GESTION RESSOURCES", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "GESTION EVENEMENTS", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "GESTION LOCATAIRES", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "GESTION EMPLOYES", nullptr));
        groupBox_3->setTitle(QString());
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        stts->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        mdf->setText(QCoreApplication::translate("MainWindow", "\342\234\216", nullptr));
        dlt->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221", nullptr));
        trr->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        trc->setItemText(0, QCoreApplication::translate("MainWindow", "Dur\303\251e", nullptr));
        trc->setItemText(1, QCoreApplication::translate("MainWindow", "Pay\303\251", nullptr));
        trc->setItemText(2, QCoreApplication::translate("MainWindow", "Impay\303\251", nullptr));
        trc->setItemText(3, QCoreApplication::translate("MainWindow", "En attente", nullptr));

        alerte->setText(QCoreApplication::translate("MainWindow", "Alerte", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
