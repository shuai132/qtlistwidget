#include "serialport.h"
#include <QSerialPortInfo>
#include <QDebug>

SerialPort::SerialPort(uint32_t baudRate, uint16_t vid, uint16_t pid)
    :vid(vid), pid(pid)
{
    serial = new QSerialPort();
    serial->setBaudRate(baudRate);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    com.SetRxPackageCallBack([](void *pParam, const BYTE * byBuf, DWORD dwLen) {
        (void)pParam;
        QByteArray bytes;
        bytes.resize(dwLen);
        for(int i=0; i<(int)dwLen; i++) {
            bytes[i] = byBuf[i];
        }
        qDebug()<<"接收到数据包:\n"<<bytes;
        emit ((SerialPort*)pParam)->onRecvPackage(bytes);
    }, this);

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
            qDebug() << "Port: " << serialPortInfo.portName() << endl
                     << "Location: " << serialPortInfo.systemLocation() << endl
                     << "Description: " << serialPortInfo.description() << endl
                     << "Manufacturer: " << serialPortInfo.manufacturer() << endl
                     << "SerialNumber: " << serialPortInfo.serialNumber() << endl
                     << "Vendor Identifier: " << serialPortInfo.vendorIdentifier() << endl
                     << "Product Identifier: " << serialPortInfo.productIdentifier() << endl
                     << "Busy: " << (serialPortInfo.isBusy() ? "Yes" : "No") << endl;

            bool valid = true;
            if (this->vid != 0) {
                if (serialPortInfo.vendorIdentifier() != this->vid) {
                    qDebug()<<"expect vid:"<<this->vid;
                    valid = false;
                    return;
                }
            }
            if (this->pid != 0) {
                if (serialPortInfo.productIdentifier() != this->pid)
                    qDebug()<<"expect pid:"<<this->pid;
                    valid = false;
                    return;
            }
            if (valid) {
                qDebug()<<"find valid serialport! opening...";
                if (serialPortInfo.isBusy()) {
                    qDebug()<<"is busy...";
                    return;
                }
                serial->setPortName(serialPortInfo.portName());
                serial->open(QIODevice::ReadWrite);
                return;
            }
        }
    }
}

void SerialPort::readData()
{
    QByteArray bytes = serial->readAll();
    emit onData(bytes);
    for (int i=0; i<bytes.length(); i++) {
        com.AnalyzePackage(bytes[i]);
    }
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
