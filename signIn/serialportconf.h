#ifndef SERIALPORTCONF_H
#define SERIALPORTCONF_H

#include <QMainWindow>
#include <QDebug>

namespace Ui {
class SerialportConf;
}

class SerialportConf : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialportConf(QWidget *parent = 0);
    ~SerialportConf();
    void saveAvailPort(QString ret);
    void dealAvailPortIsStop();
signals:
    void setBaudRate(int);
    void setParity(int);
    void setPortName(QString);
    void setDataBits(int);
    void setStopBits(int);
    void setFlowControl(int);


private slots:
    void on_pushButtonAssert_clicked();

private:
    Ui::SerialportConf *ui;
    QVector<QString> availPort;
    bool sendIsStop = true;
    size_t comcbNo = 0;
};

#endif // SERIALPORTCONF_H
