#include "relatedfunc.h"
#include "database.h"


int randNum()//返回0-9的随机数
{
    const int MIN = 0;
    const int MAX = 9;
    srand((unsigned int)time(nullptr));
    return MIN + rand() % (MAX + MIN - 1);
}

string randRace(int i)//随机产生种族
{
    if(i<2)
        return "METAL";
    else if(i<4)
        return "WOOD";
    else if(i<6)
        return "WATER";
    else if(i<8)
        return "FIRE";
    else
        return "EARTH";
}

string randCharacter(int i)//随机产生属性

{
    if(i<2)
        return "FORCE";
    else if(i<5)
        return "ENDURANCE";
    else if(i<7)
        return "DEFENSE";
    else
        return "AGILE";
}

bool checkSignIn(QString name,QString password)//登陆检查
{
    database *mysql=new database();
    bool ret=mysql->loguser(name,password);
    return ret;
}
bool checkSignUp(QString name,QString password)//注册检查
{
    database *mysql=new database();
    bool ret=mysql->signup(name,password);
    return ret;
}
bool checkLogout(QString name,QString password)//登出检查
{
    database *mysql=new database();
    bool ret=mysql->logout(name,password);
    return ret;
}

QString LoadPlayerData(QString name,QString password)//读取用户数据
{
    database *mysql=new database();
    QString ret=mysql->loadPlay(name,password);
    return ret;
}

bool SavePlayerData(QString dataString)//保存用户数据
{
    database *mysql=new database();
    return mysql->savePlay(dataString);
}

QStringList getOnlinePlayer()//获取在线用户
{
    database *mysql=new database();
    return mysql->onlinePlayer();
}
QStringList getLivePokemons()//获取所有存在精灵
{
    database *mysql=new database();
    return mysql->livePokemons();
}
QString getWorldList()//获取在线用户和存在精灵
{
    QStringList onlineplayerslist=getOnlinePlayer();
    QStringList livepokemonslist=getLivePokemons();
    QString sendData;

    //生成数据包
    for(int i=0;i<onlineplayerslist.size();i++)
        sendData=sendData+onlineplayerslist[i]+"***";

    sendData+="$$$$";

    for(int i=0;i<livepokemonslist.size();i++)
        sendData=sendData+livepokemonslist[i]+"***";

    return sendData;
}
