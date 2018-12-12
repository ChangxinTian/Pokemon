#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "tcpserver.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "******BattleServer******" << std::endl<<std::endl;

    //初始化数据库
    database ms;
    ms.initsql();

    //开始监听端口，接收客户端信息
    TcpServer ser;
    ser.listen(QHostAddress::Any,8888);

    return a.exec();
}
