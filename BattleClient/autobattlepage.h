#ifndef AUTOBATTLEPAGE_H
#define AUTOBATTLEPAGE_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include "socket.h"
#include "player.h"

namespace Ui {
class autobattlePage;
}

class autobattlePage : public QWidget
{
    Q_OBJECT

public:
    explicit autobattlePage(QWidget *parent = nullptr);
    autobattlePage(socket *s,QWidget *parent = nullptr);
    ~autobattlePage();
    void receiveSwitch(pokemon *my,pokemon *op,int num,int mypokemonnum);


signals:
    void sendBattleResult(QString re,pokemon *op,int num,int mypokemonnum);

private:
    Ui::autobattlePage *ui;
    socket *mysocket;
    QTimer *timer;

    int gameclass;//记录游戏类型
    int pokemonid;//记录对战精灵在原玩家精灵中的序号

    //实际对战精灵
    pokemon *myPokemon;
    pokemon *opPokemon;

    //原始精灵数据备份
    pokemon *myPokemonSave;
    pokemon *opPokemonSave;

    //精灵图片标签
    QLabel *myattack;
    QLabel *opattack;

    void readMassage();//获取网络数据
    void dataShow();//精灵数据展示
    void battle();//对战
    void setImage();//ui图片设置

    //攻击动画展示
    void showAttackOp();
    void showAttackMe();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // AUTOBATTLEPAGE_H
