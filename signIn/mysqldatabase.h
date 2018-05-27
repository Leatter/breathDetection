#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>

// 登录状态   成功
enum signInStatus
{
    success = 0,     // 成功
    usernameNull,    // 用户名为空
    passwordNull,    // 密码为空
    usernameError,   // 用户名错误
    passwordError    // 密码错误
};

class MySQLDatabase : public QObject
{
    Q_OBJECT
public:
    explicit MySQLDatabase(QObject *parent = 0);
    // 查询数据是否存在 如用户名
    ~MySQLDatabase();

    bool CheckExisted(const QString &type, const QString &str);
    // 插入数据
    bool insertValues(const QString &username, const QString &password,
                      const QString &name, int sex, const QString &birthday);
    // 登录
    signInStatus signIn(const QString &username, const QString &password);
    // 找回密码 检测信息
    bool checkInfo(const QString &username, const QString &name, int sex, const QString &birthday);
    // 更该密码
    bool updatePassword(const QString &type, const QString &username, const QString &newPassword);

    void queryInfo(QString username);

signals:
    void linkError(QString error);
    void sendInfo(QString name, int sex, QString birthday);

public slots:

private:
    QSqlDatabase db;
    void checkLink();
};

#endif // MYSQLDATABASE_H
