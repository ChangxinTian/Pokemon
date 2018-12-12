#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include "pokemon_creator.h"
#include "player.h"

namespace Ui {
class serversocket;
}

class serversocket : public QWidget
{
    Q_OBJECT

public:
    explicit serversocket(QWidget *parent = nullptr);
    ~serversocket();
    bool checkSignIn(QString name,QString passward);
    bool checkSignUp(QString name,QString passward);
    bool checkLogout(QString name,QString passward);
    void noviceGift(QString name,QString passward);

private slots:
    void receiveData(); //接收数据
    QString playerDataString(player_data *playerdata);
    void on_start_clicked();
    void on_stop_clicked();
    void on_test_clicked();

private:
    Ui::serversocket *ui;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    player *myplayer;
    player_data *myplaydata;
};

#endif // SERVERSOCKET_H
