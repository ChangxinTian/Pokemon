#include "welcomepage.h"
#include "ui_welcomepage.h"
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>

welcomePage::welcomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::welcomePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Welcome");
}
welcomePage::welcomePage(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::welcomePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Home");
    setWindowIcon(QIcon(":/resource/icon.png"));
    mysocket=s;
}

welcomePage::~welcomePage()
{
    delete ui;
}

void welcomePage::receiveSwitch(player_data *pd)
{
    myplayerdata=pd;

    //设置页面，展示新精灵的信息
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&welcomePage::readMassage);
    ui->label_4->setText(QString(QString::fromStdString(myplayerdata->pokemons[0].getRace())+"-")\
            +QString::fromStdString(myplayerdata->pokemons[0].getCharcter()));
    ui->label_5->setText(QString(QString::fromStdString(myplayerdata->pokemons[0].getRace())+"-")\
            +QString::fromStdString(myplayerdata->pokemons[1].getCharcter()));
    ui->label_6->setText(QString(QString::fromStdString(myplayerdata->pokemons[0].getRace())+"-")\
            +QString::fromStdString(myplayerdata->pokemons[2].getCharcter()));

    this->show();
}

void welcomePage::on_pushButton_clicked()
{
    //检查非法输入
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()){
        QMessageBox::information(this,"Error",QString::fromStdString("名字不可为空！"));
    }
    //获取输入数据，落实命名并向服务器端更新数据
    else{
        QString userName1=ui->lineEdit->text();
        QString userName2=ui->lineEdit_2->text();
        QString userName3=ui->lineEdit_3->text();
        myplayerdata->pokemons[0].setName(userName1.toStdString());
        myplayerdata->pokemons[1].setName(userName2.toStdString());
        myplayerdata->pokemons[2].setName(userName3.toStdString());

        //发送玩家更新数据包
        QString data=myplayerdata->outData();
        data="updata$$$$"+data;
        mysocket->tcpsocket->write(data.toLatin1());
    }
}

void welcomePage::readMassage()
{
    QString data=mysocket->tcpsocket->readAll();
    qDebug()<<"\nwelcomePage:"<<data;

    //解析数据包
    QStringList datalist=data.split("$$$$");
    QStringList list=datalist[0].split("#");

    if(list[0]=="updata" && list[1]=="true"){
        //命名成功，跳转到主界面
        QMessageBox::information(this,"congratulations","太棒了！开始你的游戏旅程吧！",QMessageBox::Ok);
        emit switchToHomePage(myplayerdata);
        disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&welcomePage::readMassage);
        this->hide();
    }
    else if(list[0]=="updata" && list[1]=="false"){
        QMessageBox::information(this,"sorry","宠物名字有误，再试一次吧！",QMessageBox::Ok);
    }
    else if(list[0]!="wakeup"){
        QMessageBox::information(this,"error","出现错误！",QMessageBox::Ok);
    }
}
