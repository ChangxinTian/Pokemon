#include "homepage.h"
#include "ui_homepage.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QSize>

homePage::homePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Home");

}

homePage::homePage(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Home");
    setWindowIcon(QIcon(":/resource/icon.png"));
    mysocket=s;
}

homePage::~homePage()
{
    delete ui;
}

void homePage::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    //此处可添加游戏背景
    ui->xiaozhi->setPixmap(QPixmap(":/resource/xiaozhi5.png"));
    ui->xiaozhi->setScaledContents(true);
    p.end();
}

void homePage::receiveSwitch(player_data *pd)
{
    myplayerdata=pd;
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&homePage::readMassage);
    this->show();
}

void homePage::receiveResult(QString re,pokemon *op,int num,int mypokemonnum)
{
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&homePage::readMassage);
    this->show();

    //娱乐赛结果
    if(num==1){
        if(re=="lose"){
            QMessageBox::information(this,"lose",QString::fromStdString("娱乐赛，失败了再试一次吧！"));
            myplayerdata->gamecount++;
        }
        else if(re=="win"){
            QMessageBox::information(this,"congratulations！",QString::fromStdString("娱乐赛，恭喜你赢了！"));
            myplayerdata->gamecount++;
            myplayerdata->wincount++;
        }
    }

    //升级赛结果
    else if(num==2)
    {
        if(re=="lose"){
            QMessageBox::information(this,"lose",QString::fromStdString("升级赛，失败了再试一次吧！"));
            QMessageBox message(QMessageBox::NoIcon, "congratulations！", "你出战的精灵获得20点经验！");
            message.setWindowIcon(QPixmap(":/resource/icon.png"));
            message.exec();

            //加经验和游戏场次
            if(myplayerdata->pokemons[mypokemonnum].addExperience(20)){
                QMessageBox message(QMessageBox::NoIcon, "congratulations！", QString::fromStdString(myplayerdata->pokemons[mypokemonnum].getName())+" 精灵升级啦！");
                message.setWindowIcon(QPixmap(":/resource/icon.png"));
                message.exec();
            }
            myplayerdata->gamecount++;
        }
        else if(re=="win"){
            QMessageBox::information(this,"congratulations！",QString::fromStdString("升级赛，恭喜你赢了！"));
            QMessageBox message(QMessageBox::NoIcon, "congratulations！", "你出战的精灵获得50点经验！");
            message.setWindowIcon(QPixmap(":/resource/icon.png"));
            message.exec();

            //加经验和游戏场次
            if(myplayerdata->pokemons[mypokemonnum].addExperience(50)){
                QMessageBox message(QMessageBox::NoIcon, "congratulations！", QString::fromStdString(myplayerdata->pokemons[mypokemonnum].getName())+" 精灵升级啦！");
                message.setWindowIcon(QPixmap(":/resource/icon.png"));
                message.exec();
            }
            myplayerdata->gamecount++;
            myplayerdata->wincount++;
        }
    }

    //决斗赛结果
    else if(num==3)
    {
        if(re=="lose"){
            QMessageBox::information(this,"lose",QString::fromStdString("决斗赛，失败了再试一次吧！"));
            QMessageBox message(QMessageBox::NoIcon, "congratulations！", "你出战的精灵获得20点经验！");
            message.setWindowIcon(QPixmap(":/resource/icon.png"));
            message.exec();

            //加经验和游戏场次，精灵之后删除
            if(myplayerdata->pokemons[mypokemonnum].addExperience(20)){
                QMessageBox message(QMessageBox::NoIcon, "congratulations！", QString::fromStdString(myplayerdata->pokemons[mypokemonnum].getName())+" 精灵升级啦！");
                message.setWindowIcon(QPixmap(":/resource/icon.png"));
                message.exec();
            }
            QMessageBox::information(this,"sorry",QString::fromStdString("你需要抛弃一只精灵！"));
            myplayerdata->gamecount++;
        }
        else if(re=="win"){
            QMessageBox::information(this,"congratulations！",QString::fromStdString("决斗赛，恭喜你赢了！"));
            QMessageBox message(QMessageBox::NoIcon, "congratulations！", "你出战的精灵获得50点经验！");
            message.setWindowIcon(QPixmap(":/resource/icon.png"));
            message.exec();

            //加经验
            if(myplayerdata->pokemons[mypokemonnum].addExperience(50)){
                QMessageBox message(QMessageBox::NoIcon, "congratulations！", QString::fromStdString(myplayerdata->pokemons[mypokemonnum].getName())+" 精灵升级啦！");
                message.setWindowIcon(QPixmap(":/resource/icon.png"));
                message.exec();
            }

            //获取赢得的精灵
            bool exist=true;
            for(unsigned int i=0;i<myplayerdata->pokemons.size();i++){
                if(myplayerdata->pokemons[i].getName()==op->getName())
                    exist=false;
            }
            //如果精灵已经存在，则不再加入
            if(exist==false)
                QMessageBox::information(this,"congratulations！",QString::fromStdString("你已经拥有了对战精灵，不再送出！"));
            else{
                QMessageBox::information(this,"congratulations！",QString::fromStdString("你获得了对战精灵，已放入仓库！"));
                op->setID(myplayerdata->id);
                op->setExperience(0);
                op->setStatus("live");
                myplayerdata->pokemons.push_back(*op);
            }
            //加游戏场次，玩家升级
            myplayerdata->gamecount++;
            myplayerdata->wincount++;
            myplayerdata->level++;
            if(myplayerdata->level>13)
                myplayerdata->status="10";
            else if(myplayerdata->level>8)
                myplayerdata->status="20";
            else if(myplayerdata->level>3)
                myplayerdata->status="30";
        }
    }

    //向服务器更新信息
    QString data=myplayerdata->outData();
    data="updata$$$$"+data;
    mysocket->tcpsocket->write(data.toLatin1());

    //唤出精灵删除界面
    if(num==3&&re=="lose"){
        emit switchToDeletePage(myplayerdata);
        this->hide();
        disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&homePage::readMassage);
    }
}

void homePage::readMassage()
{
    QString data=mysocket->tcpsocket->readAll();
    qDebug()<<"\nhomePage:"<<data;

    //解析数据包
    QStringList datalist=data.split("$$$$");
    QStringList list=datalist[0].split("#");

    //登出信息确认，最后发送一次updata信息，确保数据安全
    if(list[0]=="logout" && list[1]=="true"){
        //更改玩家状态
        myplayerdata->status="outline";
        QString data=myplayerdata->outData();

        //上传玩家信息，然后断开连接
        data="updata$$$$"+data;
        mysocket->tcpsocket->write(data.toLatin1());
        mysocket->tcpsocket->close();

        //显示安全登出信息
        QMessageBox::information(this,"Goodbye","已安全退出，下次再见哦!",QMessageBox::Ok);
        this->hide();
    }

    //玩家信息更新信息读取
    if(list[0]=="updata" && list[1]=="true"){
        myplayerdata->intoData(datalist[1].toStdString());
    }
}



//各种按钮槽函数

void homePage::on_MyButton_clicked()//我的（玩家信息）
{
    QMessageBox message(QMessageBox::NoIcon, "Me", "我的信息");
    QPixmap pix(":/resource/xiaozhi4.png");
    QSize picSize(150,200);
    QPixmap scaledPixmap = pix.scaled(picSize, Qt::KeepAspectRatio);
    message.setIconPixmap(scaledPixmap);
    message.setWindowIcon(scaledPixmap);
    QString aboutMe;
    aboutMe+="\nID：\t"+QString::fromStdString(myplayerdata->id);
    aboutMe+="\n";
    aboutMe+="等级：\t"+QString::number(myplayerdata->level);
    aboutMe+="\n";
    aboutMe+="生日：\t"+QString::fromStdString(myplayerdata->brithday);
    aboutMe+="\n";
    aboutMe+="游戏场次："+QString::number(myplayerdata->gamecount);
    aboutMe+="\n";
    aboutMe+="胜利场次："+QString::number(myplayerdata->wincount);
    aboutMe+="\n";
    string achieve;
    if(myplayerdata->achievement=="00")
        achieve="新手勋章";
    else if(myplayerdata->achievement=="11")
        achieve="新手勋章|宠物个数徽章（铜）";
    else if(myplayerdata->achievement=="11")
        achieve="新手勋章|宠物个数徽章（银）";
    else if(myplayerdata->achievement=="11")
        achieve="新手勋章|宠物个数徽章（金）";
    else if(myplayerdata->achievement=="21")
        achieve="新手勋章|宠物个数徽章（铜）|高级宠物徽章";
    else if(myplayerdata->achievement=="21")
        achieve="新手勋章|宠物个数徽章（银）|高级宠物徽章";
    else if(myplayerdata->achievement=="21")
        achieve="新手勋章|宠物个数徽章（金）|高级宠物徽章";
    aboutMe+="成就勋章："+QString::fromStdString(achieve);
    aboutMe+="\n";

    message.setText(aboutMe);
    message.exec();
}

void homePage::on_AboutButton_clicked()//我的
{
    //显示版本和开发者信息
    QMessageBox message(QMessageBox::NoIcon, "About", "\n版本号:1.01\n联系开发者：tcx.chn@gmail.com");
    QPixmap pix(":/resource/icon.png");
    QSize picSize(120,120);
    QPixmap scaledPixmap = pix.scaled(picSize, Qt::KeepAspectRatio);
    message.setIconPixmap(scaledPixmap);
    message.setWindowIcon(scaledPixmap);
    message.exec();

}

void homePage::on_ButtonLogout_clicked()//登出
{
    //向服务器发出登出请求
    QString sendData("logout$$$$");
    sendData = sendData+QString::fromStdString(myplayerdata->id)+"#"+QString::fromStdString(myplayerdata->password)+"#";
    mysocket->tcpsocket->write(sendData.toLatin1());
}

void homePage::on_ButtonWorld_clicked()//世界
{
    emit switchToWorldPage(myplayerdata);
    this->hide();
    disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&homePage::readMassage);
}

void homePage::on_HouseButton_clicked()//仓库
{
    emit switchToHousePage(myplayerdata);
    this->hide();
}
