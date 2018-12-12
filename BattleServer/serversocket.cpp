#include "serversocket.h"
#include "ui_serversocket.h"
#include "database.h"
#include "pokemon.h"
#include "pokemon_creator.h"
#include <QString>
#include <QTime>

serversocket::serversocket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serversocket)
{
    ui->setupUi(this);


    this->setWindowTitle("Pokemon Server");


    myplayer = new player;
    myplaydata = new player_data;

    tcpserver = new QTcpServer(this);
    tcpsocket = new QTcpSocket(this);

    connect(tcpserver,&QTcpServer::newConnection,
            [=](){
        tcpsocket = tcpserver->nextPendingConnection();

        QString ip=tcpsocket->peerAddress().toString();
        quint16 port= tcpsocket->peerPort();
        QString temp = QString("[%1:%2]成功连接").arg(ip).arg(port);
        ui->textEdit->setText(temp);
        connect(tcpsocket,&QTcpSocket::readyRead,this,&serversocket::receiveData);
        connect(tcpserver,&QTcpServer::destroyed,
                [=](){
            QString temp1=QString("[%1:%2]断开连接").arg(ip).arg(port);
            //TOGO:数据处理
        });

    });

}

serversocket::~serversocket()
{
    delete ui;
}

void serversocket::noviceGift(QString name,QString password)
{
    myplayer->setId(name.toStdString());
    myplayer->setPassword(password.toStdString());
    myplayer->setLevel(1);
    myplayer->setBrithday(QDateTime::currentDateTime());

    pokemon_creator pc;
    pokemon *p1=pc.getPokemon(WATER,1);
    pokemon *p2=pc.getPokemon(FIRE,1);
    pokemon *p3=pc.getPokemon(WOOD,1);
        qDebug()<<"Gift:"<<myplayer->getPokemons().size();
    myplayer->addPokemon(*p1);
    qDebug()<<"Gift:"<<myplayer->getPokemons().size();
    myplayer->addPokemon(*p2);
       qDebug()<<"Gift:"<<myplayer->getPokemons().size();
    myplayer->addPokemon(*p3);
       qDebug()<<"Gift:"<<myplayer->getPokemons().size();
}

void serversocket::receiveData()
{
    QString data=tcpsocket->readAll();
    ui->textEdit->append(data);

    QString dataString;
    QStringList datalist=data.split("#");
    qDebug()<<datalist;
    bool ret=0;
    if(datalist[0]=="login")
        ret=checkSignIn(datalist[1],datalist[2]);
    else if(datalist[0]=="signon"){
        ret=checkSignUp(datalist[1],datalist[2]);
        if(ret){
            noviceGift(datalist[1],datalist[2]);
            myplayer->getData(myplaydata);
            dataString = playerDataString(myplaydata);
        }
    }
    else if(datalist[0]=="logout")
        ret=checkLogout(datalist[1],datalist[2]);
    else
        //处理游戏中的数据
        return;

    QString sendData=datalist[0];
    if(ret)
        sendData+="#true#";
    else
        sendData+="#false#";
    sendData+=dataString;
    tcpsocket->write(sendData.toLatin1()); //返回信息


//    test
//    pokemon_creator pc;
//    pokemon *poke1=pc.getPokemon(WATER,2);
//    pokemon *poke2=pc.getPokemon(FIRE,3);
//    player *p=new player("Tian","1111");
//    p->addPokemon(*poke1);
//    p->addPokemon(*poke2);

//    player_data pd=p->getData();
//    sendPlay(&pd);
}

QString serversocket::playerDataString(player_data *playerdata)
{
    QString player("player");
    player = player
            +"#"+ QString::fromStdString(playerdata->id)
            +"#"+ QString::fromStdString(playerdata->password)
            +"#"+ playerdata->brithday.toString()
            +"#"+ playerdata->gametime.toString()
            +"#"+ QString::number(playerdata->level)
            +"#"+ QString::number(playerdata->pokemonNum)
            +"#" ;
    qDebug()<<"pokemon:"<<playerdata->pokemons.size();
    vector<pokemon>::iterator it;
    for(it=playerdata->pokemons.begin ();it!=playerdata->pokemons.end ();it++)
    {
        pokemon temp=*it;
        player+="pokemon";
        player = player
                +"#"+ QString::fromStdString(temp.getID())
                +"#"+ QString::number(temp.getAttack())
                +"#"+ QString::number(temp.getCharcter())
                +"#"+ QString::number(temp.getCritiacalpoint())
                +"#"+ QString::number(temp.getDefensive())
                +"#"+ QString::number(temp.getExperience())
                +"#"+ QString::number(temp.getFulllife())
                +"#"+ QString::number(temp.getInterval())
                +"#"+ QString::number(temp.getInterval_v())
                +"#"+ QString::number(temp.getLevel())
                +"#"+ QString::number(temp.getLife())
                +"#"+ QString::fromStdString(temp.getName())
                +"#"+ QString::number(temp.getRace())
                +"#";
    }
    return player;
    //tcpsocket->write(player.toLatin1());
}

bool serversocket::checkSignIn(QString name,QString password)
{
    database *mysql=new database();
    bool ret=mysql->loguser(name,password);
    return ret;
}
bool serversocket::checkSignUp(QString name,QString password)
{
    database *mysql=new database();
    bool ret=mysql->signup(name,password);
    return ret;
}
bool serversocket::checkLogout(QString name,QString password)
{
    database *mysql=new database();
    bool ret=mysql->logout(name,password);
    return ret;
}
void serversocket::on_start_clicked()
{
    ui->start->setEnabled(false);
    //监听8888端口
    if(!tcpserver->listen(QHostAddress::Any,8888)){
        qDebug()<<tcpserver->errorString();
        close();
        return;
    }
}
void serversocket::on_stop_clicked()
{
    if(nullptr==tcpsocket)
        return ;
    tcpsocket->disconnectFromHost();
    tcpsocket->close();
    ui->start->setEnabled(true);
}
void serversocket::on_test_clicked()
{
    if(nullptr==tcpsocket)
        return ;
     QString str="TEST";
     tcpsocket->write(str.toUtf8().data());
}
