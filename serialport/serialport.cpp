#include "serialport.h"
#include <QDebug>

SerialPort::SerialPort()
{
    serial = new QSerialPort();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    config();
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
    // todo:
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tryOpen()));
    timer->start(1000);
}

void SerialPort::tryOpen()
{
    qDebug()<<"try open...";
    if (!serial->isOpen()) {
        serial->open(QIODevice::ReadWrite);
    }
}

void SerialPort::readData()
{
    emit onData(serial->readAll());
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    // todo: reconnect timer
}
