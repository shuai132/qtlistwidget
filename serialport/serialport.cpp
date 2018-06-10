#include "serialport.h"
#include <QSerialPortInfo>
#include <QDebug>

SerialPort::SerialPort()
{
    serial = new QSerialPort();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    config();

    startOpenTimer();
}

SerialPort::~SerialPort()
{
    timer->stop();

    if(serial->isOpen()) {
        serial->close();
    }
    delete serial;
}

void SerialPort::config()
{
    // todo:
    serial->setPortName("/dev/ttyUSB0");
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialPort::startOpenTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tryOpen()));
    timer->start(1000);
}

void SerialPort::tryOpen()
{
    if (!serial->isOpen()) {
        qDebug()<<"try open...";
        QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &serialPortInfo : serialPortInfoList) {
            qDebug() << endl
                << QObject::tr("Port: ") << serialPortInfo.portName() << endl
                << QObject::tr("Location: ") << serialPortInfo.systemLocation() << endl
                << QObject::tr("Description: ") << serialPortInfo.description() << endl
                << QObject::tr("Manufacturer: ") << serialPortInfo.manufacturer() << endl
                << QObject::tr("Vendor Identifier: ") << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : QByteArray()) << endl
                << QObject::tr("Product Identifier: ") << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : QByteArray()) << endl
                << QObject::tr("Busy: ") << (serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) << endl;

            if (serialPortInfo.vendorIdentifier() == 0x1a86) {
                qDebug()<<"find valible serialport! opening...";
                if (serialPortInfo.isBusy()) {
                    qDebug()<<"is busy...";
                    return;
                }
                serial->setPortName(serialPortInfo.systemLocation());
                serial->open(QIODevice::ReadWrite);
                return;
            }
        }
    }
}

void SerialPort::readData()
{
    emit onData(serial->readAll());
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    qDebug()<<error;
    if (error != QSerialPort::SerialPortError::NoError) {
        if (error != QSerialPort::SerialPortError::NotOpenError)
            serial->close();
        emit onConStateChanged(false);
    }
    else {
        emit onConStateChanged(true);
    }
}
