#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include "mainwindow.h"


class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr, MainWindow *mw = nullptr);
    ~Arduino();

    void setupRFIDConnection(const QString &portName);
    void setupSurtensionConnection(const QString &portName);

    QSerialPort* getRFIDPort() const;
    QSerialPort* getSurtensionPort() const;

private slots:
    void readFromRFID();
    void readFromSurtension();

private:
    QSerialPort *rfidPort;
    QSerialPort *surtPort;
    QString rfidData;
    QString surtData;

    Arduino *arduinoManager;

    bool checkRFIDInDatabase(const QString &cardID);
    void traiterSurtension(int id);

    MainWindow *mainWindow;
};




/*
class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = nullptr);

    bool connectArduino();
    void writeToArduino(const QByteArray &data);
    QByteArray readFromArduino();
    void closeArduino();

    //void readFromArduino();
    bool checkRFIDInDatabase(const QString& cardID);

private:
    class QSerialPort *serial;
};*/

#endif // ARDUINO_H
