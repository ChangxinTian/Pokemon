#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include <QTimer>

#include "player.h"
#include "database.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = nullptr);
    ~TcpSocket();
    QByteArray handleData(QByteArray data,const QString & ip, quint16 port);//用来处理数据的函数

signals:
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的

public slots:
    void disConTcp(int i);

protected slots:
    void receiveData();//接收数据
    void startNext();//处理下一个
    void wakeUp();//定时发送

protected:
    QFutureWatcher<QByteArray> watcher;
    QQueue<QByteArray> datas;

private:
    qintptr socketID;
    QMetaObject::Connection dis;
    player_data *myplayer;//当前连接的用户
    player_data *virtualplayer;//用户进行对战的虚拟对手
    QTimer *timer;//定时器
    void forceLogout();//强制退出
};

#endif // TCPSOCKET_H
