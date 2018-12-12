#ifndef DATABASE_H
#define DATABASE_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "player.h"
#include "pokemon.h"

class database
{
public:
    database();
    void initsql();//数据库连接初始化
    void createtable(); //创造表单
    bool loguser(QString name,QString passward); //登陆
    bool signup(QString name,QString passward); //注册
    bool logout(QString name,QString passward);//登出
    QString loadPlay(QString name,QString passward);//提取玩家数据
    bool savePlay(QString dataString);//存储玩家数据
    QStringList onlinePlayer();//获取在线用户
    QStringList livePokemons();//获取所有存在精灵

private:
    QSqlQuery *query;
};

#endif // DATABASE_H
