#include "register.h"
#include "ui_register.h"
#include "widget.h"
Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("注册");
    init();
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
}

Register::~Register()
{
    delete ui;
}


void Register::on_pushButtonReturn_clicked()
{
    emit returnSignal();  
}

void Register::on_pushButtonConfirm_clicked()
{
    // 检测信息是否存在为空
    // 若信息为空 则会有提示lable显示
    on_lineEditUsername_editingFinished();
    on_lineEditPassword_editingFinished();
    on_lineEditName_editingFinished();
    on_comboBoxSex_activated(ui->comboBoxSex->currentIndex());
    // 判断提示label
    if(!ui->labelUsernameWarn->isHidden())
        return;
    if(!ui->labelPasswordWarn->isHidden())
        return;
    if(!ui->labelNameWarn->isHidden())
        return;
    if(!ui->labelSexWarn->isHidden())
        return;
    if(!ui->labelBirthdayWarn->isHidden())
        return;

    // 组包 将出生日期的形式 改为"xxxx-xx-xx"
    QDate dateEdit=ui->dateEditBirthday->date();
    QString birthday=dateEdit.toString("yyyy-MM-dd");
    int sex;
    if(ui->comboBoxSex->currentText() == "男")
        sex = 1;
    else if(ui->comboBoxSex->currentText() == "女")
        sex = 0;
    // mysql插入记录
    MySQLDatabase mysql;
    connect(&mysql, &MySQLDatabase::linkError, this, &Register::dealLinkError);
    if(mysql.insertValues(ui->lineEditUsername->text(), ui->lineEditPassword->text(), ui->lineEditName->text(),
                       sex, birthday))
    {
        if(QMessageBox::information(this, "用户注册", "注册成功", QMessageBox::Yes)
                == QMessageBox::Yes)
            emit returnSignal();
    }
    else
    {
        QMessageBox::warning(this, "用户注册", "注册失败", QMessageBox::Yes);
        return;
    }
}

void Register::init()
{
    // 隐藏提示栏
    ui->labelBirthdayWarn->hide();
    ui->labelNameWarn->hide();
    ui->labelPasswordWarn->hide();
    ui->labelSexWarn->hide();
    ui->labelUsernameWarn->hide();
    // 注册界面清屏
    ui->lineEditUsername->clear();
    ui->lineEditPassword->clear();
    ui->lineEditName->clear();
    ui->comboBoxSex->setCurrentIndex(0);
    ui->dateEditBirthday->setDateTime(QDateTime(QDate(2000, 1, 1)));

}
void Register::on_lineEditUsername_editingFinished()
{
    // 用户名为空
    if(!ui->lineEditUsername->text().size())
    {
        ui->labelUsernameWarn->setText("*用户名不能为空");
        ui->labelUsernameWarn->show();
    }
    // 用户名不为空
    else
    {
        MySQLDatabase mysql;
        connect(&mysql, &MySQLDatabase::linkError, this, &Register::dealLinkError);
        // 用户名已经存在
        if(mysql.CheckExisted(QString("username"), ui->lineEditUsername->text()))
        {
            ui->labelUsernameWarn->setText("*该用户名已存在");
            ui->labelUsernameWarn->show();
        }
        else
        {
            ui->labelUsernameWarn->hide();
        }
    }
}

void Register::on_lineEditPassword_editingFinished()
{
    if(!ui->lineEditPassword->text().size())
        ui->labelPasswordWarn->show();
    else
        ui->labelPasswordWarn->hide();
}

void Register::on_lineEditName_editingFinished()
{
    if(!ui->lineEditName->text().size())
        ui->labelNameWarn->show();
    else
        ui->labelNameWarn->hide();
}

void Register::on_comboBoxSex_activated(int index)
{
    if(index == 0)
        ui->labelSexWarn->show();
    else
        ui->labelSexWarn->hide();
}

void Register::dealLinkError(QString error)
{
    QMessageBox::warning(this, "数据库连接失败", error);
}
