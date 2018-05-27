#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QSqlDatabase>
//#include "mysql.h"
#include "mysqldatabase.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();
    void init();

signals:
    void returnSignal();

private slots:

    void on_pushButtonReturn_clicked();

    void on_pushButtonConfirm_clicked();

    void on_lineEditUsername_editingFinished();

    void on_lineEditPassword_editingFinished();

    void on_lineEditName_editingFinished();

    void on_comboBoxSex_activated(int index);

    void dealLinkError(QString error);

private:
    Ui::Register *ui;

};

#endif // REGISTER_H
