#ifndef DELETEPAGE_H
#define DELETEPAGE_H

#include <QWidget>
#include "player.h"
#include "socket.h"

namespace Ui {
class deletePage;
}

class deletePage : public QWidget
{
    Q_OBJECT

public:
    explicit deletePage(QWidget *parent = nullptr);
    deletePage(socket *s,QWidget *parent = nullptr);
    ~deletePage();
    void receiveSwitch(player_data *pd);

signals:
    void switchToHomePage(player_data *pd);

private slots:
    void on_pushButton_clicked();//确认删除按钮

private:
    Ui::deletePage *ui;
    player_data *myplayerdata;
    unsigned int n;
    socket *mysocket;
    void readMassage();
};

#endif // DELETEPAGE_H
