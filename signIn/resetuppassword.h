#ifndef RESETUPPASSWORD_H
#define RESETUPPASSWORD_H

#include <QWidget>
#include "mysqldatabase.h"
#include <QMessageBox>

namespace Ui {
class resetupPassword;
}

class resetupPassword : public QWidget
{
    Q_OBJECT

public:
    explicit resetupPassword(QWidget *parent = 0);
    ~resetupPassword();
    void init();
    // 用于窗口之间传递用户名
    void setUsername(const QString &username);
signals:
    void resetupPasswordReturn();

private slots:
    void on_lineEditNewPassword_editingFinished();

    void on_lineEditNewPasswordAgain_editingFinished();

    void on_pushButtonReturn_clicked();

    void on_pushButtonConfirm_clicked();

    void dealLinkError(QString error);

private:
    Ui::resetupPassword *ui;
    QString username;

};

#endif // RESETUPPASSWORD_H
