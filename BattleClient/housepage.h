#ifndef HOUSEPAGE_H
#define HOUSEPAGE_H

#include <QWidget>
#include "player.h"

namespace Ui {
class housePage;
}

class housePage : public QWidget
{
    Q_OBJECT

public:
    explicit housePage(QWidget *parent = nullptr);
    ~housePage();
    void receiveSwitch(player_data *pd);

signals:
    void switchToHomePage(player_data *pd);

private slots:
    void on_pushButton_clicked();//返回按钮

private:
    Ui::housePage *ui;
    player_data *myplayerdata;
};

#endif // HOUSEPAGE_H
