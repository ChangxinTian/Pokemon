#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include "relatedfunc.h"

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->timer=new QTimer;
    this->myplayer=new player_data;
    this->virtualplayer=new player_data;
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::receiveData);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            forceLogout();
            qDebug() << "disconnect" ;  
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    connect(timer, &QTimer::timeout, this,&TcpSocket::wakeUp);
    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty()) {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}

void TcpSocket::forceLogout()//强制退出函数
{
    checkLogout(QString::fromStdString(myplayer->id),QString::fromStdString(myplayer->password));
}

QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, quint16 port)//数据处理函数
{
    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100){}
    data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void  TcpSocket::wakeUp()//定时发送
{
    if(this->myplayer->status!="online")
        return;

    QStringList onlineplayerslist=getOnlinePlayer();
    QStringList livepokemonslist=getLivePokemons();

    QString sendData="wakeup";

    sendData+="$$$$";

    for(int i=0;i<onlineplayerslist.size();i++)
        sendData=sendData+onlineplayerslist[i]+"***";

    sendData+="$$$$";

    for(int i=0;i<livepokemonslist.size();i++)
        sendData=sendData+livepokemonslist[i]+"***";

    this->write(sendData.toLatin1()); //返回信息
}

void TcpSocket::receiveData()//接收并处理信息
{
    QString data=this->readAll();

    if(data=="")
        return;

    //解析数据包
    QStringList datalist=data.split("$$$$");
    qDebug()<<datalist;
    QString sendData;

    bool ret=0;
    if(datalist[0]=="login"){//处理登陆数据
        QStringList datalist2=datalist[1].split("#");
        ret=checkSignIn(datalist2[0],datalist2[1]);
        if(ret)
        {
            myplayer->intoData(LoadPlayerData(datalist2[0],datalist2[1]).toStdString());
            sendData=myplayer->outData();
        }
    }
    else if(datalist[0]=="signon"){//处理注册数据
        QStringList datalist2=datalist[1].split("#");
        ret=checkSignUp(datalist2[0],datalist2[1]);
        if(ret){
            myplayer->noviceGift(datalist2[0],datalist2[1]);
            sendData+=myplayer->outData();
        }
    }
    else if(datalist[0]=="logout"){//处理登出数据
        QStringList datalist2=datalist[1].split("#");
        ret=checkLogout(datalist2[0],datalist2[1]);
    }
    else if(datalist[0]=="updata"){//处理更新数据
        myplayer->intoData(datalist[1].toStdString());
        ret=SavePlayerData(myplayer->outData());
        myplayer->intoData(LoadPlayerData(QString::fromStdString(myplayer->id),QString::fromStdString(myplayer->password)).toStdString());
        sendData=myplayer->outData();
    }
    else if(datalist[0]=="worldlist"){//处理世界列表数据
        sendData=getWorldList();
        ret=true;
    }
    else if(datalist[0]=="opponent"){//处理请求对战数据
        virtualplayer->intoData(LoadPlayerData("virtual","virtual").toStdString());
        sendData=virtualplayer->outData();
        ret=true;
    }
    else if(datalist[0]=="delete"){//处理精灵删除数据
        myplayer->intoData(datalist[1].toStdString());
        if(myplayer->pokemons.size()==0)
            myplayer->gift();
        ret=SavePlayerData(myplayer->outData());
        myplayer->intoData(LoadPlayerData(QString::fromStdString(myplayer->id),QString::fromStdString(myplayer->password)).toStdString());
        sendData=myplayer->outData();
    }
    else{
        //处理游戏中的其他数据
    }

    //合成数据包并发送
    QString sendMark;
    sendMark=datalist[0];
    if(ret)
        sendData=sendMark+"#true$$$$"+sendData;
    else
        sendData=sendMark+"#false$$$$"+sendData;

    this->write(sendData.toLatin1()); //返回信息
}

