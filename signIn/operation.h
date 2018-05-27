#ifndef OPERATION_H
#define OPERATION_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "mysqldatabase.h"
#include "widget.h"
#include "serialportconf.h"
#include "SubThread.h"

namespace Ui {
class operation;
}

class operation : public QMainWindow
{
    Q_OBJECT

public:
    explicit operation(QWidget *parent = 0);
    ~operation();
    void ReceiveUsername(std::string username);
    void DealTouristSignIn();
    void dealLinkError(QString error);
    void dealUserInfo(QString name, int sex, QString birthday);

    //static QPixmap PixmapToRound(QPixmap &src, int radius);
private slots:
    void on_SerialPortConf_clicked();

    void dealPortStatus(bool flag);

    void on_pushButtonStart_toggled(bool checked);

    void realtimeDataSlot();

    void saveToQVector(int num);

    void on_pushButton_clicked();

signals:
    void buildConnection();
    void stopConnection();
    void checkAvailPort();
private:
    Ui::operation *ui;
    QTimer* dataTimer;
    Widget* w;

    /* multiThread operation */
    QThread *thread;
    SubThread *myThread;

    /* store user info */
    struct{
        std::string* username;
        int sex;
        std::string birthday, name;
    }userInfo;

    /* serial port set */
    SerialportConf spc;

    /* */
    struct{
        QVector<int> ivec, data, toHandleData;
        double index = 0;
        QVector<QString> avilPort;
        int sendDataNumber = 0;

        // variant for adaptive threshold
        int timeCount = 0; // 记录一个时间窗耗费的时间
        int peakNum = 0, valleyNum = 0;
        QVector<int> maxArray, minArray;
    }plot;

    void setupCustomPlot(QCustomPlot *customPlot);
    void mutilThread_operation();
};

#endif // OPERATION_H
