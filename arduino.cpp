#include "arduino.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent)
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
}
