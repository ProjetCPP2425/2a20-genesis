#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>

class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = nullptr);

    bool connectArduino();
    void writeToArduino(const QByteArray &data);
    QByteArray readFromArduino();
    void closeArduino();

private:
    class QSerialPort *serial;
};

#endif // ARDUINO_H
