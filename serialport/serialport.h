#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    SerialPort();
    ~SerialPort();

private:
    void config();
    void startOpenTimer();

private slots:
    void tryOpen();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void onData(QByteArray byteArray);

private:
    QSerialPort* serial = nullptr;
    bool isOpened = false;
    QTimer* timer = nullptr;
};

#endif // SERIALPORT_H
