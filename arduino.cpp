#include <QThread>  // Required for QThread::msleep()
#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

Arduino::~Arduino()
{
    delete serial;
}

int Arduino::connect_arduino()
{
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            qDebug() << "Found device:" << serial_port_info.portName()
            << "VID:" << serial_port_info.vendorIdentifier()
            << "PID:" << serial_port_info.productIdentifier();

            if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }

    qDebug() << "Arduino port name:" << arduino_port_name;

    if(arduino_is_available) {
        serial->setPortName(arduino_port_name);

        if(serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            // Wait for port to initialize
            QThread::msleep(100);  // Now this will work

            qDebug() << "Connected to Arduino on" << arduino_port_name;
            return 0;
        }
        qDebug() << "Failed to open port" << arduino_port_name;
        return 1;
    }

    qDebug() << "Arduino not found";
    return -1;
}

int Arduino::close_arduino()
{
    if(serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}

QByteArray Arduino::read_from_arduino()
{
    if(serial->isReadable()) {
        data = serial->readAll();
        while(serial->waitForReadyRead(10)) {
            data += serial->readAll();
        }
        qDebug() << "Received:" << data;
        return data;
    }
    qDebug() << "Port not readable";
    return QByteArray();
}

void Arduino::write_to_arduino(QByteArray d)
{
    if(serial->isOpen() && serial->isWritable()) {
        serial->write(d);
        if(serial->waitForBytesWritten(1000)) {
            qDebug() << "Sent:" << d;
        } else {
            qDebug() << "Write timeout";
        }
    } else {
        qDebug() << "Port not writable";
    }
}

QSerialPort* Arduino::getserial()
{
    return serial;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}
