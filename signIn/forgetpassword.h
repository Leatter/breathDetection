#ifndef FORGETPASSWORD_H
#define FORGETPASSWORD_H

#include <QWidget>
#include "mysqldatabase.h"
#include <QMessageBox>
#include "resetuppassword.h"
namespace Ui {
class ForgetPassword;
}

class ForgetPassword : public QWidget
{
    Q_OBJECT

public:
    explicit ForgetPassword(QWidget *parent = 0);
    ~ForgetPassword();
    void init();

signals:
    void returnSignal();

private slots:
    void on_pushButtonReturn_clicked();

    void on_pushButtonFindPassword_clicked();

    void on_lineEditUsername_editingFinished();

    void on_lineEditName_editingFinished();

    void on_comboBoxSex_activated(int index);

    void dealresetupPasswordReturn();

    void dealLinkError(QString error);

private:
    Ui::ForgetPassword *ui;

    resetupPassword rp;
};

#endif // FORGETPASSWORD_H
