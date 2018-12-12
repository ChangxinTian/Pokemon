#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <string>
#include <iostream>
#include <QTcpSocket>
#include "socket.h"
#include "player.h"

using namespace std;

#define ip "127.0.0.1"
#define port 8888

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    Widget(socket *s,QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_login_clicked();//登陆
    void on_signon_clicked();//注册

signals:
    void switchToHomePage(player_data *pd);
    void switchToWelcomePage(player_data *pd);

private:
    Ui::Widget *ui;
    socket *mysocket;
    player_data *myplayerdata;//本地保存用户数据
    void readMassage();//接收服务器的信息
};

#endif // WIDGET_H
