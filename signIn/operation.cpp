#include "operation.h"
#include "ui_operation.h"

operation::operation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::operation)
{
    ui->setupUi(this);
    setupCustomPlot(ui->customPlot);
    this->hide();
    QPixmap icon(":/new/prefix1/C:/Users/Public/Pictures/Sample Pictures/carl-cerstrand-136810-unsplash.jpg");
    QPixmap fit = icon.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fit = Widget::PixmapToRound(fit, 45);
//    ui->labelIcon->setFixedSize(411, 161);
//    ui->labelIcon->setScaledContents(true);
    ui->labelIcon->setPixmap(fit);
    w = new Widget;
    w->show();
    // set connections
    mutilThread_operation();
    connect(w, &Widget::sendUsername,  this, &operation::ReceiveUsername);
    connect(w, &Widget::TouristSignIn, this, &operation::DealTouristSignIn);

    connect(&spc, &SerialportConf::setPortName,    myThread, &SubThread::setPortName);
    connect(&spc, &SerialportConf::setBaudRate,    myThread, &SubThread::setBaudRate); 
    connect(&spc, &SerialportConf::setStopBits,    myThread, &SubThread::setStopBits);
    connect(&spc, &SerialportConf::setDataBits,    myThread, &SubThread::setDataBits);
    connect(&spc, &SerialportConf::setFlowControl, myThread, &SubThread::setFlowControl);
    connect(&spc, &SerialportConf::setParity,      myThread, &SubThread::setParity);
    connect(this, &operation::buildConnection,     myThread, &SubThread::openSerialPort);
    connect(this, &operation::stopConnection,      myThread, &SubThread::closeSerialPort);
    connect(this, &operation::checkAvailPort,      myThread, &SubThread::checkAvailPort);

    connect(myThread, &SubThread::sendPortStatus, this, &operation::dealPortStatus);
    connect(myThread, &SubThread::sendToQVector,  this, &operation::saveToQVector);
    connect(myThread, &SubThread::sendAvailPortIsStop, &spc, &SerialportConf::dealAvailPortIsStop);
    connect(myThread, &SubThread::sendAvailPort,  &spc, &SerialportConf::saveAvailPort);

}

operation::~operation()
{
    delete ui;
}

void operation::setupCustomPlot(QCustomPlot *customPlot)
{
    // set QCustomPlot transparent
    // ui->customPlot->setAttribute(Qt::WA_OpaquePaintEvent, false);
    // ui->customPlot->setStyleSheet("background:hsva(255,255,255,0%);");
    // ui->customPlot->setBackground(QBrush(Qt::NoBrush));
    ui->customPlot->addGraph(); // blue line

    // set title of plot:
    ui->customPlot->plotLayout()->insertRow(0);
    QCPTextElement *pTextElement = new QCPTextElement(customPlot,QString("Respritory Curve"),
                                                      QFont("Times New Roman", 15, -1, true));
    pTextElement->setMargins(QMargins(0,20,0,0));

    ui->customPlot->plotLayout()->addElement(0, 0, pTextElement);
    //customPlot->addGraph(); // white line
    QPen pen(QColor(255, 110, 40));
    pen.setWidth(2);
    ui->customPlot->graph(0)->setPen(pen);
    ui->customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(QColor(0, 40, 40)));

    //QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    //timeTicker->setTimeFormat("%h:%m:%s");
    //ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    //ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 2));

    ui->customPlot->yAxis->setRange(1, 2.5);

    ui->customPlot->xAxis->setLabel(QString("t/s"));
    // set font
    ui->customPlot->xAxis->setLabelFont(QFont(QFont("Times New Roman", 12, -1, true)));
    ui->customPlot->xAxis->setTickLabelFont(QFont("Times New Roman",12));
    ui->customPlot->yAxis->setLabel(QString("U/V"));
    ui->customPlot->yAxis->setLabelFont(QFont(QFont("Times New Roman", 12, -1, true)));
    ui->customPlot->yAxis->setTickLabelFont(QFont("Times New Roman",12));
    // hide grid
    //ui->customPlot->xAxis->grid()->setVisible(false);
    //ui->customPlot->yAxis->grid()->setVisible(false);
    // 设置坐标轴距离边框的距离
    ui->customPlot->yAxis2->setPadding(60);
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    dataTimer = new QTimer(this);
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(dataTimer, &QTimer::timeout, this, &operation::realtimeDataSlot);
    // Interval 0 means to refresh as fast as possible
}

#define SETFONT(widget) \
    do{\
    QFont font;\
    font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));\
    font.setPointSize(14);\
    font.setBold(false);\
    font.setItalic(false);\
    font.setWeight(50);\
    widget->setFont(font);\
    }while(0)


void operation::ReceiveUsername(std::string username)
{
    this->userInfo.username = new std::string(username);
    MySQLDatabase mysql;
    connect(&mysql, &MySQLDatabase::linkError, this, &operation::dealLinkError);
    connect(&mysql, &MySQLDatabase::sendInfo, this, &operation::dealUserInfo);
    mysql.queryInfo(QString(this->userInfo.username->c_str()));
    ui->Name->setText(QString(this->userInfo.name.c_str()));
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    ui->Age->setText(QString::number(QString(current_date.toStdString().substr(0,4).c_str()).toInt()
                                     - QString(this->userInfo.birthday.c_str()).toInt()));
    //qDebug() << current_date << "  " << ui->Age->text();
    if(ui->Sex) ui->Sex->setText(QString("男"));
    else ui->Sex->setText(QString("女"));
    SETFONT(ui->Name); SETFONT(ui->Sex);
    // TODO: add sign in animation
    this->show();
}

void operation::DealTouristSignIn()
{
    this->show();
}

void operation::dealLinkError(QString error)
{
     QMessageBox::warning(this, "数据库连接失败", error);
}

void operation::dealUserInfo(QString name, int sex, QString birthday)
{
    this->userInfo.name = name.toStdString();
    this->userInfo.sex = sex;
    this->userInfo.birthday = birthday.toStdString().substr(0, 4);
    // qDebug() << sex << this->birthday.c_str();
}

void operation::on_SerialPortConf_clicked()
{
    emit checkAvailPort();
    spc.show();
}

void operation::mutilThread_operation()
{
    // MutilThread : New a thread
    // Warning : Don't set parent with myThread and thread
    myThread = new SubThread;
    thread = new QThread;
    myThread->moveToThread(thread);

    // Once thread start, it will trigged myThread::init
    connect(thread, &QThread::started, myThread, &SubThread::SerialPortInit);

    // Thread start
    thread->start();

    // Once qApp is about to quit, it will trigged myThread::deleteLater
    connect(qApp, &QCoreApplication::aboutToQuit, myThread, &QObject::deleteLater);

    // Once mythread destoryed, it will trigged thread::quit
    connect(myThread, &QObject::destroyed, thread, &QThread::quit);

    // Once thread finished, it will trigged thread::deleteLater
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
}

void operation::dealPortStatus(bool flag)
{
    if(flag == false)
    {
        QMessageBox::warning(this,QString("串口打开失败"),
            QString("串口不存在或本串口已经被占用，请重试！"),
            QMessageBox::Cancel);
    }
}

void operation::on_pushButtonStart_toggled(bool checked)
{
    if(checked == true) {
        if(!dataTimer->isActive())   dataTimer->start(50);  // 绘图用的定时器
        emit buildConnection();
        ui->pushButtonStart->setText(QString("暂停测试"));
    }
    else {
        if(dataTimer->isActive())  dataTimer->stop();
        emit stopConnection();
        ui->pushButtonStart->setText(QString("开始测试"));
    }
}

void operation::realtimeDataSlot()
{
//    if(plot.ivec.size() >= 3)
//    {
//        int i = 0;
//        for(i = 1; i < plot.ivec.size() - 1; i++)
//        {
////            if(plot.ivec[i] >= plot.ivec[i-1] && (plot.ivec[i] > plot.ivec[i+1]) && plot.ivec[i] > 1.7/3.26*1023)
////            {
////                QCPItemEllipse *ellipse = new QCPItemEllipse(ui->customPlot);
////                ellipse->setPen(QPen(QColor(40, 110, 255)));
////                ellipse->setAntialiased(true);
////                ellipse->topLeft->setCoords(index+0.5, plot.ivec[i]/1023.0*3.26+0.01);
////                ellipse->bottomRight->setCoords(index+1.5, plot.ivec[i]/1023.0*3.26-0.01);
////            }
//            ui->customPlot->graph(0)->addData(++index, plot.ivec[i]/1023.0*3.26);
//            //qDebug() << plot.ivec[i]/1023.0*3.26;

//        }
//        ui->customPlot->xAxis->setRange(index, 100, Qt::AlignRight);
//        ui->customPlot->replot();
//        int temp1 = plot.ivec[i-1];
//        int temp2 = plot.ivec[i];
//        plot.ivec.clear();
//        plot.ivec.push_back(temp1);
//        plot.ivec.push_back(temp2);
//    }
    while(plot.index < plot.ivec.size())
    {
        plot.index++;
        ui->customPlot->graph(0)->addData(plot.index/5.0, plot.ivec[plot.index - 1]/1023.0*3.26);
        //if(index >= 7)
        //{
            //qDebug() << toHandleData[index - 7];
            //ui->customPlot->graph(1)->addData(index/5.0, toHandleData[index - 7]/7/1023.0*3.26);
        //}
        //qDebug() << index;
        ui->customPlot->xAxis->setRange(plot.index/5, 60, Qt::AlignRight);
        ui->customPlot->replot();
    }
    //plot.ivec.clear();
}

void operation::saveToQVector(int num)
{
    plot.ivec.push_back(num);
    plot.data.push_back(num);
    int sum = 0;
    if(plot.ivec.size() >= 7)
    {
        for(int i = plot.ivec.size() - 7; i <= plot.ivec.size() - 1; i++)
            sum += plot.ivec[i];
    //qDebug() << sum;
    plot.toHandleData.push_back(sum);
    }
    double peakAverage = 0.0, valleyAverage = 0.0;
    int number = 0;  // 时间窗内呼吸波的个数
    // adaptive threshold
    if(plot.toHandleData.size() >= 3)
    {
        plot.timeCount++;
        // 判断极大值
        if(plot.toHandleData[plot.toHandleData.size() - 2] > plot.toHandleData[plot.toHandleData.size() -3 ] &&
                plot.toHandleData[plot.toHandleData.size() - 2] >= plot.toHandleData[plot.toHandleData.size() - 1])
            if(plot.toHandleData[plot.toHandleData.size() - 2] > 1.7 / 3.26 * 1023 *7) // TODO: 确保滤波后的极大值点的电压大于1.7V
                plot.maxArray.push_back(plot.toHandleData[plot.toHandleData.size() - 2]);
        if(plot.toHandleData[plot.toHandleData.size() - 2] < plot.toHandleData[plot.toHandleData.size() - 3] &&
                plot.toHandleData[plot.toHandleData.size() - 2] <= plot.toHandleData[plot.toHandleData.size() - 1])  // 判断极大值
            if(plot.toHandleData[plot.toHandleData.size() - 2] < 1.65 / 3.26 * 1023 *7) // TODO: 确保滤波后的极小值点的电压小于1.65V
                plot.minArray.push_back(plot.toHandleData[plot.toHandleData.size() - 2]);
        if((plot.minArray.size() + plot.maxArray.size()) == 6)  // 6为时间窗长度
        {
            peakAverage = 1.0/plot.maxArray.size()*(std::accumulate(plot.maxArray.begin(), plot.maxArray.end(), 0));
            valleyAverage = 1.0/plot.minArray.size()*(std::accumulate(plot.minArray.begin(), plot.minArray.end(), 0));
            for(int i = 0; i < plot.minArray.size(); i++)
                if(plot.minArray[i] <= 0.3 * peakAverage + 0.7 * valleyAverage)
                    plot.valleyNum ++;
            for(int i = 0; i < plot.maxArray.size(); i++)
                if(plot.maxArray[i] >= 0.7 * peakAverage + 0.3 * valleyAverage)
                    plot.peakNum ++;
            plot.maxArray.clear();
            plot.minArray.clear();

            qDebug() << plot.timeCount;
            if(plot.timeCount > 150 && plot.timeCount < 200)  // 约 30S
            {

                number = (plot.valleyNum + plot.peakNum) / 2;
                plot.valleyNum = 0;
                plot.peakNum = 0;
                //qDebug() << number;
                ui->RR->setText(QString::number(300 * number / plot.timeCount));
                //ui->RespitoryRate->display(300 * number / timeCount);  // 呼吸率 = 60 * num / (采样时间 * timeCount) ;
                plot.timeCount = 0;
            }
        }
        if(plot.timeCount >= 200)  // TODO： 确认200是否合适
        {
            ui->RR->setText(QString::number(0));
            plot.timeCount = 0;
        }
    }
    //qDebug() << num;
}

void operation::on_pushButton_clicked()
{
    QWidget *w = new QWidget;
    this->hide();
    w->show();
}
