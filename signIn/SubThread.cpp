#include "SubThread.h"

SubThread::SubThread(QObject *parent) : QObject(parent)
{

}

void SubThread::SerialPortInit()
{
    mySerialPort = new QSerialPort(this);
    connect(mySerialPort, &QSerialPort::readyRead, this, &SubThread::readMyCom);
    //mySerialPort->open(QIODevice::ReadWrite);
    checkAvailPort();
    //qDebug() << QString("SubThread: ") << QThread::currentThread();
}

void SubThread::setBaudRate(int index)
{
    switch(index)
    {
    case 0:
        mySerialPort->setBaudRate(QSerialPort::Baud115200);
        break;
    case 1:
        mySerialPort->setBaudRate(QSerialPort::Baud57600);
        break;
    case 2:
        mySerialPort->setBaudRate(QSerialPort::Baud38400);
        break;
    case 3:
        mySerialPort->setBaudRate(QSerialPort::Baud19200);
        break;
    case 4:
        mySerialPort->setBaudRate(QSerialPort::Baud9600);
        break;
    case 5:
        mySerialPort->setBaudRate(QSerialPort::Baud4800);
        break;
    case 6:
        mySerialPort->setBaudRate(QSerialPort::Baud2400);
        break;
    case 7:
        mySerialPort->setBaudRate(QSerialPort::Baud1200);
        break;
    }
}

void SubThread::setParity(int index)
{
    //qDebug() << "Parity " << index;
    switch(index)
    {
    case 0:
        mySerialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        mySerialPort->setParity(QSerialPort::OddParity);
        break;
    case 2:
        mySerialPort->setParity(QSerialPort::EvenParity);
        break;
    }
}

void SubThread::readMyCom()
{
    QByteArray data = mySerialPort->readAll();
    //QThread::msleep(0);
    //QByteArray data = mySerialPort->read(50);
    emit sendToTextBrowser(data);
    if(!data.isEmpty())
    {
        data_buffer += data;
        //qDebug() << data_buffer.toHex();
        if(data_buffer.length() > 16)
        {
            int i = 0;
            for(; i < data_buffer.length() - 3; i++) // 一次只发送一个数据
            {
                if(((unsigned char)data_buffer.at(i) == 0xFA) && ((unsigned char)data_buffer.at(i+1) == 0xFA))
                {
                    int num = (unsigned char)data_buffer.at(i+2) * 256 + (unsigned char)data_buffer.at(i+3);
                    //qDebug() << num;
                    if(num > 376 && num < 716)
                    {
                        emit sendToQVector(num);
                    }
                    break;
                }
            }
            data_buffer = data_buffer.mid(i + 16, data_buffer.length() - 16 - i);
            //qDebug() << sizeof(data_buffer);
        }
    }
    //qDebug() << data;
}

void SubThread::setPortName(QString portName)
{
    //QString portName = QString("COM%1").arg(index);
    //qDebug() << portName;
    mySerialPort->setPortName(portName);
}

void SubThread::setDataBits(int index)
{
    //qDebug() << "Data Bits"<< index;
    switch(index)
    {
    case 0:
        mySerialPort->setDataBits(QSerialPort::Data8);
        break;
    case 1:
        mySerialPort->setDataBits(QSerialPort::Data7);
        break;
    case 2:
        mySerialPort->setDataBits(QSerialPort::Data6);
        break;
    case 3:
        mySerialPort->setDataBits(QSerialPort::Data5);
        break;
    }
}

void SubThread::setStopBits(int index)
{
    //qDebug() << "Stop Bits"<< index;
    switch(index)
    {
    case 0:
        mySerialPort->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        mySerialPort->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        mySerialPort->setStopBits(QSerialPort::TwoStop);
        break;
    }
}

void SubThread::setFlowControl(int index)
{
    switch(index)
    {
    case 0:
        mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
        break;
    case 1:
        mySerialPort->setFlowControl(QSerialPort::HardwareControl);
        break;
    case 2:
        mySerialPort->setFlowControl(QSerialPort::SoftwareControl);
        break;
    }
}

void SubThread::openSerialPort()
{
    if(mySerialPort->open(QIODevice::ReadWrite))
    {
        portIsOpen = true;
    }
    else
        portIsOpen = false;
    emit sendPortStatus(portIsOpen);
}

void SubThread::closeSerialPort()
{
    mySerialPort->close();
}

void SubThread::checkAvailPort()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
        qDebug() << "System Location: " << info.systemLocation();
        emit sendAvailPort(info.portName());
    }
    emit sendAvailPortIsStop();
}
