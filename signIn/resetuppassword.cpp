#include "resetuppassword.h"
#include "ui_resetuppassword.h"

resetupPassword::resetupPassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resetupPassword)
{
    ui->setupUi(this);
    setWindowTitle("设置新密码");
    ui->labelWarn->hide();
    init();
    ui->lineEditNewPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditNewPasswordAgain->setEchoMode(QLineEdit::Password);
}

resetupPassword::~resetupPassword()
{
    delete ui;
}

void resetupPassword::on_lineEditNewPassword_editingFinished()
{
    if(!ui->lineEditNewPassword->text().size())
    {
        ui->labelWarn->setText("*密码不能为空");
        ui->labelWarn->show();
    }
    else
    {
        ui->labelWarn->hide();
    }
}

void resetupPassword::on_lineEditNewPasswordAgain_editingFinished()
{
    if(!ui->lineEditNewPasswordAgain->text().size())
    {
        ui->labelWarn->setText("*密码不能为空");
        ui->labelWarn->show();
    }
    else
    {
        if(ui->lineEditNewPassword->text() != ui->lineEditNewPasswordAgain->text())
        {
            ui->labelWarn->setText("*两次输入的密码不一致");
            ui->labelWarn->show();
        }
        else
            ui->labelWarn->hide();
    }
}

void resetupPassword::on_pushButtonReturn_clicked()
{
    emit resetupPasswordReturn();
}

void resetupPassword::on_pushButtonConfirm_clicked()
{
    on_lineEditNewPassword_editingFinished();
    on_lineEditNewPasswordAgain_editingFinished();
    if(!ui->labelWarn->isHidden()) return;
    // 新密码更新到数据库
    //qDebug() << username;
    MySQLDatabase mysql;
    connect(&mysql, &MySQLDatabase::linkError, this, &resetupPassword::dealLinkError);
    if(mysql.updatePassword("username", username, ui->lineEditNewPassword->text()))
    {
        QMessageBox::information(this, "设置新密码", "密码重置成功");
    }
    else
    {
        QMessageBox::warning(this, "设置新密码", "密码重置失败");
        return;
    }
    emit resetupPasswordReturn();
}
void resetupPassword::init()
{
    ui->lineEditNewPassword->clear();
    ui->lineEditNewPasswordAgain->clear();
}

void resetupPassword::setUsername(const QString &username)
{
    this->username = username;
}

void resetupPassword::dealLinkError(QString error)
{
    QMessageBox::warning(this, "数据库连接失败", error);
}
