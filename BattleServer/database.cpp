#include "database.h"

database::database()
{

}

void database::initsql()//数据库连接初始化
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("test");
    if(db.open())
        {
            qDebug()<<"Database connected successfully!";
            createtable();
            return;
        }
    else
        {
            qDebug()<<"Database connected failed!";
            return;
        }
}

void database::createtable()//创造表单
{
    //如果已经有table，则不会重复创建
    query=new QSqlQuery;
    query->exec("create table user(id VARCHAR(30) PRIMARY KEY UNIQUE NOT NULL,password VARCHAR(30),status VARCHAR(10),level INT,brithday VARCHAR(30),achievement VARCHAR(20),gamecount INT,wincount INT)");
    query->exec("insert into user values('root','root','root','0','root','000000000000000','0','0')");
    query->exec("create table pokemon(name VARCHAR(30),id VARCHAR(30),status VARCHAR(10),attack INT,defensive INT,charact VARCHAR(20),critiacalpoint INT,experience INT,fulllife INT,interal INT,level INT,life INT,race VARCHAR(30),PRIMARY KEY (`name`,`id`))");
    query->exec("insert into pokemon value('root', 'root','root','0','0','root','0','0','0','0','0','0','root')");
}

bool database::loguser(QString name, QString password)//登陆
{
    //检查是否有此账户
    QString str=QString("select * from user where id='%1' and password='%2' and status='outline'").arg(name).arg(password);
    query=new QSqlQuery;
    query->exec(str);
    query->last();
    int record=query->at()+1;
    if(record==0)
        return false;

    //更新数据至在线
    QString upd=QString("update user set status='online' where id='%1'and password='%2'").arg(name).arg(password);
    bool ret = query->exec(upd);

    return ret;
}

bool database::signup(QString name,QString passward)//注册
{
    //检查是否已有该用户
    QString str=QString("select * from user where id='%1'").arg(name);
    query=new QSqlQuery;
    query->exec(str);
    query->last();
    int record=query->at()+1;
    if(record!=0)
        return false;
    return true;
}

bool database::logout(QString name,QString passward)//登出
{
    query=new QSqlQuery;

    //检查是否有此账户
    QString str=QString("select * from user where id='%1' and password='%2'").arg(name).arg(passward);
    query->exec(str);
    query->last();
    int record=query->at()+1;
    if(record==0)
        return false;

    //更新数据，使下线
    QString upd=QString("update user set status='outline' where id='%1'and password='%2'").arg(name).arg(passward);
    bool ret = query->exec(upd);
    return ret;
}

QString database::loadPlay(QString id,QString password)//提取玩家数据
{
    player_data pd;
    query=new QSqlQuery;
    QString ret;

    //读取玩家数据部分
    QString str1=QString("select * from user where id='%1' and password='%2'").arg(id).arg(password);
    query->exec(str1);
    while(query->next()){
        ret = "player#";
        ret+=query->value(0).toString()+"#";//id
        ret+=query->value(1).toString()+"#";//密码
        ret+=query->value(2).toString()+"#";//在线状态
        ret+=query->value(3).toString()+"#";//level
        ret+=query->value(4).toString()+"#";//生日
        ret+=query->value(5).toString()+"#";//成就
        ret+=query->value(6).toString()+"#";//游戏场次
        ret+=query->value(7).toString();//胜利场次
    }
    ret+="***";

    //读取精灵数据部分
    QString str2=QString("select * from pokemon where id='%1' and status='live'").arg(id);
    query->clear();
    query->exec(str2);
    while(query->next()){
        ret+="pokemon#";
        ret+=query->value(0).toString()+"#";//name
        ret+=query->value(1).toString()+"#";//id
        ret+=query->value(2).toString()+"#";//status
        ret+=query->value(3).toString()+"#";//attack
        ret+=query->value(4).toString()+"#";//defensive
        ret+=query->value(5).toString()+"#";//character
        ret+=query->value(6).toString()+"#";//critiacalpoint
        ret+=query->value(7).toString()+"#";//experience
        ret+=query->value(8).toString()+"#";//fulllife
        ret+=query->value(9).toString()+"#";//inter
        ret+=query->value(10).toString()+"#";//level
        ret+=query->value(11).toString()+"#";//life
        ret+=query->value(12).toString();//race
        ret+="***";
    }
    return ret;
}

bool database::savePlay(QString dataString)//存储玩家数据
{
    player_data pd;
    query=new QSqlQuery;

    QStringList datalist=dataString.split("***");
    QStringList playerString=datalist[0].split("#");

    //检查是否有此用户，有则更新，没有则插入
    QString str1=QString("select * from user where id='%1' and password='%2'").arg(playerString[1]).arg(playerString[2]);
    query->clear();
    query->exec(str1);
    query->last();
    int record=query->at()+1;
    if(record==0){
        QString str2=QString("insert into user values('%1','%2','%3','%4','%5','%6','%7','%8')")\
                .arg(playerString[1]).arg(playerString[2]).arg(playerString[3]).arg(playerString[4]).arg(playerString[5])\
                .arg(playerString[6]).arg(playerString[7]).arg(playerString[8]);
        query->clear();
        query->exec(str2);
    }
    else{
        QString str2= QString("update user set status='%1',level='%2', brithday='%3',achievement='%4',gamecount='%5',wincount='%6' where id='%7'and password='%8'")\
                .arg(playerString[3]).arg(playerString[4]).arg(playerString[5]).arg(playerString[6]).arg(playerString[7])\
                .arg(playerString[8]).arg(playerString[1]).arg(playerString[2]);
        query->clear();
        query->exec(str2);

    }

    //逐个检查是否有该精灵，有则更新，没有则插入
    for(int i=1;i+1<datalist.size();i++){
        QStringList pokemonString = datalist[i].split("#");
        QString str3=QString("select * from pokemon where name='%1'and id='%2'").arg(pokemonString[1]).arg(pokemonString[2]);//error
        query->clear();
        query->exec(str3);
        query->last();
        int recordTemp=query->at()+1;
        if(recordTemp==0){
            QString str2=QString("insert into pokemon values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13')")\
                    .arg(pokemonString[1]).arg(pokemonString[2]).arg(pokemonString[3]).arg(pokemonString[4]).arg(pokemonString[5])\
                    .arg(pokemonString[6]).arg(pokemonString[7]).arg(pokemonString[8]).arg(pokemonString[9]).arg(pokemonString[10])\
                    .arg(pokemonString[11]).arg(pokemonString[12]).arg(pokemonString[13]);
            query->clear();
            query->exec(str2);
        }
        else{
            QString str2=QString("update pokemon set status='%1',attack='%2',defensive='%3',charact='%4',critiacalpoint='%5',experience='%6',fulllife='%7',interal='%8',level='%9',life='%10',race='%11'where name='%12'and id='%13'")\
                    .arg(pokemonString[3]).arg(pokemonString[4]).arg(pokemonString[5]).arg(pokemonString[6]).arg(pokemonString[7])\
                    .arg(pokemonString[8]).arg(pokemonString[9]).arg(pokemonString[10]).arg(pokemonString[11]).arg(pokemonString[12])\
                    .arg(pokemonString[13]).arg(pokemonString[1]).arg(pokemonString[2]);
            query->clear();
            query->exec(str2);
        }
    }

    return true;
}

QStringList database::onlinePlayer()//获取在线用户
{
    QStringList onlineplayer;

    query=new QSqlQuery;
    QString str=QString("select * from user where status='online'");
    query->clear();
    query->exec(str);
    while(query->next()){
        onlineplayer << QString(query->value(0).toString()+"#"+query->value(3).toString()+"#"+query->value(4).toString()+"#"+query->value(5).toString()+"#"+query->value(6).toString()+"#"+query->value(7).toString());
    }

    return onlineplayer;
}

QStringList database::livePokemons()//获取所有存在精灵
{
    QStringList livepokemon;

    query=new QSqlQuery;
    QString str=QString("select * from pokemon where status='live'");
    query->clear();
    query->exec(str);
    while(query->next()){
        livepokemon << QString(query->value(0).toString()+"#"+query->value(1).toString()+"#"+query->value(10).toString()+"#"+query->value(12).toString());
    }

    return livepokemon;
}



