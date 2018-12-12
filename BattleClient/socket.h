#ifndef SOCKET_H
#define SOCKET_H

#include<QTcpSocket>
#include<QDebug>

using namespace std;

#define myip "127.0.0.1"
#define myport 8888

class socket:public QObject
{
public:
    socket();
    QTcpSocket *tcpsocket;
    void initTcp();//初始化tcp
private:

};

#endif // SOCKET_H
