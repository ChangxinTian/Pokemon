#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include "socket.h"
#include "player.h"

namespace Ui {
class homePage;
}

class homePage : public QWidget
{
    Q_OBJECT

public:
    explicit homePage(QWidget *parent = nullptr);
    homePage(socket *s,QWidget *parent = nullptr);
    ~homePage();
    void receiveSwitch(player_data *pd);
    void receiveResult(QString re,pokemon *op,int num,int mypokemonnum);
private slots:
    void on_ButtonLogout_clicked();//登出
    void on_ButtonWorld_clicked();//世界
    void on_AboutButton_clicked();//关于
    void on_MyButton_clicked();//我的
    void on_HouseButton_clicked();//仓库

signals:
    void switchToWorldPage(player_data *pd);
    void switchToHousePage(player_data *pd);
    void switchToDeletePage(player_data *pd);
    //对战精灵选择页面，数字代表对战类型，1为娱乐赛，2为升级赛，3为决斗赛
    void switchToChoicePage(player_data *pd,int num);

private:
    Ui::homePage *ui;
    socket *mysocket;
    player_data *myplayerdata;
    void readMassage();//数据接收

protected:
    void paintEvent(QPaintEvent *);//窗口重绘函数
};

#endif // HOMEPAGE_H
