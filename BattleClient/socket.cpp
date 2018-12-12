#include "socket.h"

socket::socket()
{
       tcpsocket=nullptr;
}

void socket::initTcp()
{
    tcpsocket = new QTcpSocket;

    //清空原有数据
    tcpsocket->abort();
    //与服务器建立连接
    tcpsocket->connectToHost(myip,myport);
    //打印Debug信息
    connect(tcpsocket,&QTcpSocket::connected,
            [=](){
        qDebug()<<"网络连接成功";
    });
}
