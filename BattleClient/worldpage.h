#ifndef WORLDPAGE_H
#define WORLDPAGE_H

#include <QWidget>
#include "socket.h"
#include "player.h"

namespace Ui {
class worldPage;
}

class worldPage : public QWidget
{
    Q_OBJECT

public:
    explicit worldPage(QWidget *parent = nullptr);
    worldPage(socket *s,QWidget *parent = nullptr);
    ~worldPage();
    void receiveSwitch(player_data *pd);

private slots:
    void on_pushButton_clicked();

signals:
    void switchToHomePage(player_data *pd);

private:
    Ui::worldPage *ui;
    socket *mysocket;
    player_data *myplayerdata;
    void readMassage();
    void setUserTable(QString data);//设置在线用户表
    void setPokemonTable(QString data);//设置存在精灵表
};

#endif // WORLDPAGE_H
