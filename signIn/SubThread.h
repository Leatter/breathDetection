#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QByteArray>

class SubThread : public QObject
{
    Q_OBJECT
public:
    // SubThread construct function
    explicit SubThread(QObject *parent = 0);

    // serial port initialize
    void SerialPortInit();

    // set serial port BaudRate
    void setBaudRate(int index);

    // set serial port Parity
    void setParity(int index);

    // set serial port name
    void setPortName(QString portName);

    // set serial port databits
    void setDataBits(int index);

    // set serial port stopbits
    void setStopBits(int index);

    // set serial port flowcontrol
    void setFlowControl(int index);

    // call QSerialPort::open
    void openSerialPort();

    // call QSerialPort::close
    void closeSerialPort();

    // slot for serialport receive data and then send to mainThread
    void readMyCom();

    void checkAvailPort();
signals:

    void sendPortStatus(bool);
    void sendAvailPort(QString);
    void sendToTextBrowser(QByteArray);
    void sendToQVector(int);
    void sendAvailPortIsStop();

public slots:


private:
    QSerialPort *mySerialPort;
    bool portIsOpen = false;
    QByteArray data_buffer;

};

#endif // SUBTHREAD_H
