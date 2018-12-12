#ifndef RELATEDFUNC_H
#define RELATEDFUNC_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <string>
#include <QDateTime>

using namespace std;

int randNum();//返回0-9的随机数

string randRace(int i);//随机产生种族
string randCharacter(int i);//随机产生属性

bool checkSignIn(QString name,QString password);//登陆检查
bool checkSignUp(QString name,QString password);//注册检查
bool checkLogout(QString name,QString password);//登出检查

QString LoadPlayerData(QString name,QString password);//读取用户数据
bool SavePlayerData(QString dataString);//保存用户数据

QStringList getOnlinePlayer();//获取在线用户
QStringList getLivePokemons();//获取所有存在精灵
QString getWorldList();//获取在线用户和存在精灵

#endif // RELATEDFUNC_H
