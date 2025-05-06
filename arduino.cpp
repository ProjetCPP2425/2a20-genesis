#include "arduino.h"
#include "qsqlerror.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include "mainwindow.h"

Arduino::Arduino(QObject *parent,MainWindow *mw) : QObject(parent),
    rfidPort(new QSerialPort(this)),
    surtPort(new QSerialPort(this)) {}

Arduino::~Arduino() {}

void Arduino::setupRFIDConnection(const QString &portName) {
    rfidPort->setPortName(portName);
    rfidPort->setBaudRate(QSerialPort::Baud9600);
    rfidPort->setDataBits(QSerialPort::Data8);
    rfidPort->setParity(QSerialPort::NoParity);
    rfidPort->setStopBits(QSerialPort::OneStop);
    rfidPort->setFlowControl(QSerialPort::NoFlowControl);

    if (rfidPort->open(QIODevice::ReadWrite)) {
        connect(rfidPort, &QSerialPort::readyRead, this, &Arduino::readFromRFID);
    } else {
        qDebug() << " Failed to open RFID port:" << rfidPort->errorString();
    }
}

void Arduino::setupSurtensionConnection(const QString &portName) {
    surtPort->setPortName(portName);
    surtPort->setBaudRate(QSerialPort::Baud9600);
    surtPort->setDataBits(QSerialPort::Data8);
    surtPort->setParity(QSerialPort::NoParity);
    surtPort->setStopBits(QSerialPort::OneStop);
    surtPort->setFlowControl(QSerialPort::NoFlowControl);

    if (surtPort->open(QIODevice::ReadWrite)) {
        connect(surtPort, &QSerialPort::readyRead, this, &Arduino::readFromSurtension);
    } else {
        qDebug() << " Failed to open Surtension port:" << surtPort->errorString();
    }
}

QSerialPort* Arduino::getRFIDPort() const { return rfidPort; }
QSerialPort* Arduino::getSurtensionPort() const { return surtPort; }



void Arduino::readFromRFID() {
    rfidData += rfidPort->readAll();

    while (rfidData.contains('\n')) {
        int newlineIndex = rfidData.indexOf('\n');
        QString line = rfidData.left(newlineIndex);
        rfidData.remove(0, newlineIndex + 1);

        qDebug() << "Raw line from Arduino (before trim):" << line.toUtf8();
        line = line.trimmed();
        qDebug() << "Trimmed line:" << line.toUtf8();

        if (line.isEmpty() || line == "NO_CARD" || line.startsWith("Received command:")) {
            qDebug() << "Ignored line";
            continue;
        }

        QString normalizedCardID = line.toUpper().remove(' ').remove('\r');
        qDebug() << "Normalized card ID to check:" << normalizedCardID;

        if (checkRFIDInDatabase(normalizedCardID)) {
            qDebug() << "Card recognized!";
            rfidPort->write("servo\n");
            rfidPort->flush();
            QMessageBox::information(nullptr, "Access Granted", "Welcome");
        } else {
            qDebug() << "Card NOT recognized!";

            rfidPort->write("buzz\n");
            rfidPort->flush();
            QMessageBox::warning(nullptr, "Access Denied", "Card not recognized.");
        }
    }

}

bool Arduino::checkRFIDInDatabase(const QString& cardID) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM LOCATAIRES WHERE CODE = :cardID");
    query.bindValue(":cardID", cardID);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}




void Arduino::readFromSurtension()
{
    surtData += rfidPort->readAll();

    if (surtData.contains('\n')) {
        QString message = surtData.trimmed();
        qDebug() << "Message reçu :" << message;

        if (message.startsWith("SURTENSION")) {
            // Exemple de message reçu : SURTENSION;ID=3
            QStringList parts = message.split(';');
            if (parts.size() == 2 && parts[1].startsWith("ID=")) {
                QString idString = parts[1].section('=', 1, 1);
                int id = idString.toInt();
                if (id > 0) {
                    traiterSurtension(id);
                }
            }

            // Action facultative : envoyer confirmation à Arduino
            rfidPort->write("COUPER_ALIM\n");
        }

        surtData.clear(); // important de vider pour le prochain message
    }
}



void Arduino::traiterSurtension(int id)
{
    QSqlQuery query;

    QString today = QDate::currentDate().toString("dd-MM-yyyy"); // Date du jour

    query.prepare("UPDATE IMENE0RESSOURCES SET ETATR = 'En panne', DM = TO_DATE(:date, 'DD-MM-YYYY') WHERE IDR = :id");
    query.bindValue(":date", today);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✅ Ressource ID" << id << "mise à jour : En panne.";

        mainWindow->afficherRessources(); // instead of afficherRessources()
         // Refresh du tableau

        // 1. On récupère le NOM de la ressource
        QSqlQuery getNameQuery;
        getNameQuery.prepare("SELECT NOMR FROM ressources WHERE IDR = :id");
        getNameQuery.bindValue(":id", id);

        QString nomRessource = "Inconnu";

        if (getNameQuery.exec() && getNameQuery.next()) {
            nomRessource = getNameQuery.value(0).toString();
        }

        // 2. Envoyer à Arduino
        QString message = "NOM:" + nomRessource + "\n"; // ➔ Ex : NOM:Ordinateur\n
        rfidPort->write(message.toUtf8()); // on envoie en format texte UTF8

        // 3. Popup Qt
        QMessageBox::warning(nullptr, "SURTENSION DÉTECTÉE",
                             "La ressource '" + nomRessource + "' est passée à l'état 'En panne' !");
    } else {
        qDebug() << "❌ Erreur de mise à jour : " << query.lastError().text();
    }
}







//////////////////////////////////////////////////////////////
/*Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);

}

bool Arduino::connectArduino()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.vendorIdentifier() && info.productIdentifier()) {
            serial->setPort(info);
            serial->setBaudRate(QSerialPort::Baud9600);
            if (serial->open(QIODevice::ReadWrite)) {
                qDebug() << "Arduino connected on:" << info.portName();
                return true;
            }
        }
    }
    qDebug() << "Arduino not found.";
    return false;
}

void Arduino::writeToArduino(const QByteArray &data)
{
    if (serial->isWritable()) {
        serial->write(data);
    }
}

QByteArray Arduino::readFromArduino()
{
    QByteArray data;
    if (serial->isReadable()) {
        data = serial->readAll();
    }
    return data;
}

void Arduino::closeArduino()
{
    if (serial->isOpen()) {
        serial->close();
    }
}*/
