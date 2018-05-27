#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include "register.h"
#include "forgetpassword.h"
#include "mysqldatabase.h"
#include <fstream>
#include <sstream>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    // LabelIcon设置为圆形方框  TODO: 图片缩小后会出现锯齿
    static QPixmap PixmapToRound(QPixmap &src, int radius);


private slots:
    void on_pushButtonRegister_clicked();
    void dealRegisterReturn();
    void dealFindPasswordReturn();
    void on_pushButtonFindPassword_clicked();

    void on_pushButtonUser_clicked();

    void dealLinkError(QString error);

    void on_checkBoxAutoSignIn_clicked();

    void on_pushButtonTourist_clicked();

signals:
    void sendUsername(std::string username);
    void TouristSignIn();

private:
    Ui::Widget *ui;
    Register reg;
    ForgetPassword fp;

};

#endif // WIDGET_H
