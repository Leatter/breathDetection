#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    QPixmap icon(":/new/prefix1/C:/Users/Public/Pictures/Sample Pictures/carl-cerstrand-136810-unsplash.jpg");
    QPixmap fit = icon.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fit = PixmapToRound(fit, 45);
//    ui->labelIcon->setFixedSize(411, 161);
//    ui->labelIcon->setScaledContents(true);
    ui->labelIcon->setPixmap(fit);

    connect(&reg, &Register::returnSignal, this, &Widget::dealRegisterReturn);
    //connect(&reg, &Register::regSuccess, this, &Widget::dealRegisterReturn);
    connect(&fp, &ForgetPassword::returnSignal, this, &Widget::dealFindPasswordReturn);
    // 图标透明 + 不规则
//    ui->pushButtonUser->setStyleSheet("QPushButton{border:0px;}");
//    ui->pushButtonUser->setIcon(QPixmap("C:/Users/Public/Pictures/Sample Pictures/2.jpg"));
//    ui->pushButtonUser->setIconSize(QPixmap("C:/Users/Public/Pictures/Sample Pictures/2.jpg").size());
//    ui->pushButtonUser->resize(QPixmap("C:/Users/Public/Pictures/Sample Pictures/2.jpg").size());
    std::ifstream fin("signInRecord.dat");
    bool remIsChecked = false, autoIsChecked = false;
    std::string str1, str2;
    fin >> remIsChecked >> autoIsChecked >> str1 >> str2;
    if(remIsChecked)
    {
        ui->checkBoxRemPassword->setChecked(true);
        ui->lineEditUsername->setText(QString::fromStdString(str1));
        ui->lineEditPassword->setText(QString::fromStdString(str2));
    }
    if(autoIsChecked)
    {
        ui->checkBoxAutoSignIn->setChecked(true);
        on_pushButtonUser_clicked();
    }
}

Widget::~Widget()
{
    delete ui;
}

QPixmap Widget::PixmapToRound(QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void Widget::on_pushButtonRegister_clicked()
{
    this->hide();
    reg.show();
}

void Widget::dealRegisterReturn()
{
    reg.hide();
    reg.init();
    this->show();
}

void Widget::dealFindPasswordReturn()
{
    fp.hide();
    fp.init();
    this->show();
}

void Widget::on_pushButtonFindPassword_clicked()
{
    this->hide();
    fp.show();
}

void Widget::on_pushButtonUser_clicked()
{
    MySQLDatabase mysql;
    connect(&mysql, &MySQLDatabase::linkError, this, &Widget::dealLinkError);
    if(mysql.signIn(ui->lineEditUsername->text(), ui->lineEditPassword->text()) == success)
    {
        // TODO: 进入串口设置界面
        //qDebug() << 1;
        // 保存用户名和密码到文件  TODO: 存储时加密
        std::string str1 = ui->lineEditUsername->text().toStdString();
        std::string str2 = ui->lineEditPassword->text().toStdString();
        std::ofstream fout("signInRecord.dat");
        fout << ui->checkBoxRemPassword->isChecked() << " "  << ui->checkBoxAutoSignIn->isChecked()
             << " " << str1 << " " << str2;
        fout.close();
        this->close();
        emit sendUsername(str1);
    }
    else if(mysql.signIn(ui->lineEditUsername->text(), ui->lineEditPassword->text()) == usernameNull)
    {
        QMessageBox::warning(this, "用户注册", "用户名不能为空",QMessageBox::Yes);
        return;
    }
    else if(mysql.signIn(ui->lineEditUsername->text(), ui->lineEditPassword->text()) == passwordNull)
    {
        QMessageBox::warning(this, "用户注册", "密码不能为空",QMessageBox::Yes);
        return;
    }
    else if(mysql.signIn(ui->lineEditUsername->text(), ui->lineEditPassword->text()) == usernameError)
    {
        QMessageBox::warning(this, "用户注册", "用户名不存在",QMessageBox::Yes);
        return;
    }
    else if(mysql.signIn(ui->lineEditUsername->text(), ui->lineEditPassword->text()) == passwordError)
    {
        QMessageBox::warning(this, "用户注册", "密码错误",QMessageBox::Yes);
        return;
    }
}

void Widget::dealLinkError(QString error)
{
     QMessageBox::warning(this, "数据库连接失败", error);
}

void Widget::on_checkBoxAutoSignIn_clicked()
{
    ui->checkBoxRemPassword->setChecked(true);
}

void Widget::on_pushButtonTourist_clicked()
{
    // TODO:
    //qDebug() << 1;
    emit TouristSignIn();
}
