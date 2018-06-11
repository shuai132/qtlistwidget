#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(uint32_t baudRate, uint16_t vid = 0, uint16_t pid = 0);
    ~SerialPort();

private:
    void config();
    void startOpenTimer();

private slots:
    void tryOpen();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void onConStateChanged(bool isConnected);
    void onData(QByteArray byteArray);

private:
    QSerialPort* serial = nullptr;
    bool isOpened = false;
    QTimer* timer = nullptr;

    // expected VID and PID of USB device
    uint16_t vid;
    uint16_t pid;
};

#endif // SERIALPORT_H
