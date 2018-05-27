#include "serialportconf.h"
#include "ui_serialportconf.h"

SerialportConf::SerialportConf(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialportConf)
{
    ui->setupUi(this);
}

SerialportConf::~SerialportConf()
{
    delete ui;
}

void SerialportConf::on_pushButtonAssert_clicked()
{
    emit setPortName(ui->COMCB->currentText());
    emit setBaudRate(ui->BAUDCB->currentIndex());
    emit setDataBits(ui->DataBitsCB->currentIndex());
    emit setStopBits(ui->StopBitsCB->currentIndex());
    emit setParity(ui->ParityCB->currentIndex());
    emit setFlowControl(ui->FlowCtrlCB->currentIndex());
    this->hide();
}

void SerialportConf::saveAvailPort(QString ret)
{
    availPort.push_back(ret);
    //ui->COMCB->clear();
    // 信号是一个一个来的 ui->comcb 不能再槽中设置
//    if(ret != "")
//        ui->COMCB->insertItem(comcbNo++, ret);

}
void SerialportConf::dealAvailPortIsStop()
{
    ui->COMCB->clear();
    for(int i = 0; i < availPort.size(); i++)
    {
        ui->COMCB->insertItem(i, availPort[i]);
    }
    availPort.clear();
}
