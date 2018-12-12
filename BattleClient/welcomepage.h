#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include "socket.h"
#include "player.h"

namespace Ui {
class welcomePage;
}

class welcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit welcomePage(QWidget *parent = nullptr);
    welcomePage(socket *s,QWidget *parent = nullptr);
    ~welcomePage();
    void receiveSwitch(player_data *pd);//接收到界面转换

private slots:
    void on_pushButton_clicked();//确定命名按钮

signals:
    void switchToHomePage(player_data *pd);

private:
    Ui::welcomePage *ui;
    socket *mysocket;
    player_data *myplayerdata;//暂时保存玩家数据
    void readMassage();
};

#endif // WELCOMEPAGE_H
