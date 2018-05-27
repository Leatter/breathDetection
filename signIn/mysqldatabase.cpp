#include "mysqldatabase.h"

MySQLDatabase::MySQLDatabase(QObject *parent) : QObject(parent)
{
    if(QSqlDatabase::contains("db"))
        db = QSqlDatabase::database("db");
    else
        db = QSqlDatabase::addDatabase("QMYSQL","db");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("qxy940816");
    db.setDatabaseName("respitorydata");
}

bool MySQLDatabase::CheckExisted(const QString &type, const QString &str)
{
    checkLink();
    QSqlQuery query(db);
    query.exec(QString("SELECT %1 FROM info WHERE %1 = '%2';").arg(type).arg(str));
    bool isExisted = false;
    while(query.next())
    {
        if(query.value(type).toString() == str)
            isExisted = true;
    }
    return isExisted;
}

bool MySQLDatabase::insertValues(const QString &username, const QString &password,
                                const QString &name, int sex, const QString &birthday)
{
    checkLink();
    QSqlQuery query(db);
    return query.exec(QString("INSERT info(username,password,name,sex,birthday) "
                                   "VALUES('%1','%2','%3',%4, '%5');").arg(username)
                                   .arg(password).arg(name).arg(sex).arg(birthday));
}

signInStatus MySQLDatabase::signIn(const QString &username, const QString &password)
{
    checkLink();
    if(username == "") return usernameNull;
    if(password == "") return passwordNull;
    QSqlQuery query(db);
    query.exec(QString("SELECT username, password FROM info "
                       "WHERE username = '%1';")
                       .arg(username));
    bool usernameExisted = false, passwordExisted = false;
    while(query.next())
    {
        if(query.value("username").toString() == username)
        {
            if(query.value("password").toString() == password)
            {
                usernameExisted = true;
                passwordExisted = true;
                break;
            }
            else
                usernameExisted = true;
        }
    }
    if(usernameExisted)
        if(passwordExisted)
            return success;
        else
            return passwordError;
    else
        return usernameError;
}

bool MySQLDatabase::checkInfo(const QString &username, const QString &name, int sex,
                              const QString &birthday)
{
    checkLink();
    QSqlQuery query(db);
    query.exec(QString("SELECT name, sex, birthday FROM info WHERE username = '%1';").arg(username));
    bool isExisted = false;
    while(query.next())
    {
        if(query.value("name").toString() == name && query.value("sex").toInt() == sex
                && query.value("birthday").toString() == birthday)
            isExisted = true;
    }
    return isExisted;
}

bool MySQLDatabase::updatePassword(const QString &type, const QString &username, const QString &newPassword)
{
    checkLink();
    QSqlQuery query(db);
    return query.exec(QString("UPDATE info SET password = '%1' WHERE %2 = '%3'")
                      .arg(newPassword).arg(type).arg(username));
}

void MySQLDatabase::checkLink()
{
    if(!db.open())
    {
        //qDebug() << db.lastError().text();
        emit linkError(db.lastError().text());
    }
}

void MySQLDatabase::queryInfo(QString username)
{
    checkLink();
    QSqlQuery query(db);
    query.exec(QString("SELECT name, sex, birthday FROM info WHERE username = '%1';").arg(username));
    while(query.next())
    {
        //qDebug() << query.value("birthday").toString();
        emit sendInfo(query.value("name").toString(), query.value("sex").toInt(), query.value("birthday").toString());
    }
}
MySQLDatabase::~MySQLDatabase()
{
    db.close();
}
