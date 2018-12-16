#ifndef CHOICEPAGE_H
#define CHOICEPAGE_H

#include <QWidget>
#include "socket.h"
#include "player.h"

namespace Ui {
class choicePage;
}

class choicePage : public QWidget
{
    Q_OBJECT

public:
    explicit choicePage(QWidget *parent = nullptr);
    choicePage(socket *s,QWidget *parent = nullptr);
    ~choicePage();
    void receiveSwitch(player_data *pd,int num);

signals:
    void switchToHomePage(player_data *pd);
    void switchToBattlePage(pokemon *my,pokemon *op);
    void switchToAutobattlePage(pokemon *my,pokemon *op,int num,int mypokemonnum);

private slots:
    void on_backButton_clicked();//返回主界面
    void on_okButton_clicked();//确认选择

private:
    Ui::choicePage *ui;
    int gameclass;
    socket *mysocket;
    player_data *myplayerdata;//我的数据
    player_data *opponentdata;//我要对战的虚拟玩家数据
    void readMassage();
    void setOpponentTable();//设置对手的精灵表
    void setPokemonTable();//设置我的精灵表
};

#endif // CHOICEPAGE_H
