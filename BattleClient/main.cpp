#include "widget.h"
#include <QApplication>
#include <QObject>
#include <QFile>

#include "homepage.h"
#include "welcomepage.h"
#include "worldpage.h"
#include "socket.h"
#include "player.h"
#include "pokemon.h"
#include "housepage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //初始化tcp，和服务器建立连接
    socket *s=new socket;
    s->initTcp();

    //登陆注册界面
    Widget widget(s);
    widget.show();
    //新手欢迎界面
    welcomePage welcome(s);
    //主界面
    homePage home(s);
    //世界界面（显示在线用户和存在的精灵）
    worldPage world(s);
    //仓库界面，显示我拥有的精灵
    housePage house;

    //信号连接部分
    QObject::connect(&widget,&Widget::switchToHomePage,&home,&homePage::receiveSwitch);
    QObject::connect(&widget,&Widget::switchToWelcomePage,&welcome,&welcomePage::receiveSwitch);
    QObject::connect(&welcome,&welcomePage::switchToHomePage,&home,&homePage::receiveSwitch);
    QObject::connect(&home,&homePage::switchToWorldPage,&world,&worldPage::receiveSwitch);
    QObject::connect(&world,&worldPage::switchToHomePage,&home,&homePage::receiveSwitch);
    QObject::connect(&home,&homePage::switchToHousePage,&house,&housePage::receiveSwitch);
    QObject::connect(&house,&housePage::switchToHomePage,&home,&homePage::receiveSwitch);

    return a.exec();
}
