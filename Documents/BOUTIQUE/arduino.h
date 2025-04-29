#ifndef ARDUINO_H
#define ARDUINO_H

#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>


#include <QtSerialPort/QSerialPort> // classe rassemblant des fonctions permettant l'échange des données dans une liasion série
#include <QtSerialPort/QSerialPortInfo> // classe forunissant des informations sur les ports disponibles


class Arduino
{
public:
    Arduino();
    int connect_arduino(); // permet de connecter le PC à Arduino
    int close_arduino(); // permet de femer la connexion
    void write_to_arduino( QByteArray ); // envoyer des données vers arduino
    QByteArray read_from_arduino(); //recevoir des données de la carte Arduino
    QSerialPort* getserial();
    QString getarduino_port_name();

private:
    QSerialPort * serial; //Cet objet rassemble des informations (vitesse, bits de données, etc.)
    //et des fonctions (envoi, lecture de réception, ... ) sur ce qu'est une voie série pour Arduino.
    static const quint16 arduino_uno_vendor_id=9025;
    static const quint16 arduino_uno_producy_id=67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data; // contenant les données lues à partir d'Arduino
};





/*class ArduinoData;

class Arduino : public QMainWindow
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Arduino(QWidget *parent = nullptr);
    Arduino(const Arduino &);
    Arduino(Arduino &&);
    Arduino &operator=(const Arduino &);
    Arduino &operator=(Arduino &&);
    ~Arduino();

signals:

private:
    QSharedDataPointer<ArduinoData> data;
};*/

#endif // ARDUINO_H
