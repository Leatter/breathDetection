#include "forgetpassword.h"
#include "ui_forgetpassword.h"

ForgetPassword::ForgetPassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForgetPassword)
{
    ui->setupUi(this);
    this->setWindowTitle("找回密码");    
    init();
    rp.hide();
    connect(&rp, &resetupPassword::resetupPasswordReturn, this, &ForgetPassword::dealresetupPasswordReturn);
}

ForgetPassword::~ForgetPassword()
{
    delete ui;
}

void ForgetPassword::on_pushButtonReturn_clicked()
{
    emit returnSignal();
}

void ForgetPassword::on_pushButtonFindPassword_clicked()
{
    // 检测信息是否存在为空
    // 若信息为空 则会有提示lable显示
    on_lineEditUsername_editingFinished();
    on_lineEditName_editingFinished();
    on_comboBoxSex_activated(ui->comboBoxSex->currentIndex());
    // 判断提示label
    if(!ui->labelUsernameWarn->isHidden())
        return;
    if(!ui->labelNameWarn->isHidden())
        return;
    if(!ui->labelSexWarn->isHidden())
        return;
    if(!ui->labelBirthdayWarn->isHidden())
        return;
    // 组包 将出生日期的形式 改为"xxxx-xx"
    QDate dateEdit=ui->dateEditBirthday->date();
    QString birthday = dateEdit.toString("yyyy-MM-dd");
    // mysql插入记录
    int sex;
    if(ui->comboBoxSex->currentText() == "男")
        sex = 1;
    else if(ui->comboBoxSex->currentText() == "女")
        sex = 0;
    MySQLDatabase mysql;
    connect(&mysql, &MySQLDatabase::linkError, this, &ForgetPassword::dealLinkError);
    if(mysql.checkInfo(ui->lineEditUsername->text(), ui->lineEditName->text(), sex, birthday))
    {
        rp.setUsername(ui->lineEditUsername->text());
        this->hide();
        this->init();
        rp.show();
    }
    else
    {
        QMessageBox::warning(this, "找回密码", "信息不匹配");
    }
}

void ForgetPassword::on_lineEditUsername_editingFinished()
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
        connect(&mysql, &MySQLDatabase::linkError, this, &ForgetPassword::dealLinkError);
        // 用户名不存在
        if(mysql.CheckExisted(QString("username"), ui->lineEditUsername->text()))
        {
            ui->labelUsernameWarn->hide();
        }
        else
        {
            ui->labelUsernameWarn->setText("*该用户名不存在");
            ui->labelUsernameWarn->show();
        }
    }
}

void ForgetPassword::dealLinkError(QString error)
{
    QMessageBox::warning(this, "数据库连接失败", error);
}

void ForgetPassword::on_lineEditName_editingFinished()
{
    if(ui->lineEditName->text() == "")
        ui->labelNameWarn->show();
    else
        ui->labelNameWarn->hide();
}

void ForgetPassword::on_comboBoxSex_activated(int index)
{
    if(index == 0)
        ui->labelSexWarn->show();
    else
        ui->labelSexWarn->hide();
}

void ForgetPassword::dealresetupPasswordReturn()
{
    emit returnSignal();
    rp.hide();
    rp.init();
}

void ForgetPassword::init()
{
    ui->lineEditName->clear();
    ui->lineEditUsername->clear();
    ui->comboBoxSex->setCurrentIndex(0);
    ui->dateEditBirthday->setDateTime(QDateTime(QDate(2000, 1, 1)));
    // 隐藏提示栏
    ui->labelBirthdayWarn->hide();
    ui->labelNameWarn->hide();
    ui->labelSexWarn->hide();
    ui->labelUsernameWarn->hide();
}
