 #ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include "socket.h"
#include "player.h"

namespace Ui {
class battlePage;
}

class battlePage : public QWidget
{
    Q_OBJECT

public:
    explicit battlePage(QWidget *parent = nullptr);
    battlePage(socket *s,QWidget *parent = nullptr);
    ~battlePage();
    void receiveSwitch(pokemon *my,pokemon *op);

signals:
    void sendBattleResult(QString re,pokemon *op,int num,int mypokemonnum);

private:
    Ui::battlePage *ui;
    socket *mysocket;
    QTimer *timer;
    int mytime;

    //实际对战精灵
    pokemon *myPokemon;
    pokemon *opPokemon;

    //精灵图片标签
    QLabel *myattack;
    QLabel *opattack;

    //原始精灵数据备份
    pokemon *myPokemonSave;
    pokemon *opPokemonSave;

    void readMassage();//获取网络数据
    void dataShow();//精灵数据展示
    void battle();//对战
    void setImage();//ui图片设置

    //攻击动画展示
    void showAttackOp();
    void showAttackMe();

private slots:
    void on_PuGong_clicked();//普攻
    void on_JiNeng_clicked();//技能

protected:
    void paintEvent(QPaintEvent *);
};

#endif // BATTLEPAGE_H
