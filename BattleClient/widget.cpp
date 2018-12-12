#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Pokemon");
    ui->lineEditpsw->setEchoMode(QLineEdit::Password);

    mysocket->tcpsocket = new QTcpSocket(this);
    mysocket->tcpsocket->abort();

    mysocket->tcpsocket->connectToHost(ip,port);
    connect(mysocket->tcpsocket,&QTcpSocket::connected,
            [=](){
        QMessageBox::information(this,"网络连接",QString::fromStdString("网络已连接！"));
    });
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&Widget::readMassage);

}
Widget::Widget(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //ui初始设置
    setWindowIcon(QIcon(":/resource/icon.png"));
    this->setWindowTitle("Pokemon");
    ui->lineEditpsw->setEchoMode(QLineEdit::Password);

    //继承连接
    mysocket=s;
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&Widget::readMassage);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_clicked()
{
    //用于Debug
    qDebug()<<ui->lineEdituser->text();
    qDebug()<<ui->lineEditpsw->text();

    //检查非法输入
    if(ui->lineEdituser->text().isEmpty()){
        QMessageBox::information(this,"Error",QString::fromStdString("账号不可为空！"));
    }
    else if(ui->lineEditpsw->text().isEmpty()){
        QMessageBox::information(this,"Error",QString::fromStdString("密码不可为空！"));
    }
    else{
        //输入合法，发出登陆申请，包括用户名和密码
        QString userName=ui->lineEdituser->text();
        QString passward=ui->lineEditpsw->text();
        QString bs="login";
        QString data=bs+"$$$$"+userName+"#"+passward;

        mysocket->tcpsocket->write(data.toLatin1());

        ui->lineEdituser->clear();
        ui->lineEditpsw->clear();
    }
}

void Widget::on_signon_clicked()
{
    //用于Debug
    qDebug()<<ui->lineEdituser->text();
    qDebug()<<ui->lineEditpsw->text();

    //检查非法输入
    if(ui->lineEdituser->text().isEmpty()){
        QMessageBox::information(this,"Error",QString::fromStdString("账号不可为空！"));
    }
    else if(ui->lineEditpsw->text().isEmpty()){
        QMessageBox::information(this,"Error",QString::fromStdString("密码不可为空！"));
    }
    else{
        //输入合法，发出注册申请，包括用户名和密码
        QString userName=ui->lineEdituser->text();
        QString passward=ui->lineEditpsw->text();
        QString bs="signon";
        QString data=bs+"$$$$"+userName+"#"+passward;

        mysocket->tcpsocket->write(data.toLatin1());

        ui->lineEdituser->clear();
        ui->lineEditpsw->clear();
    }
}

void Widget::readMassage()
{
    QString data=mysocket->tcpsocket->readAll();
    qDebug()<<"\nwidgetPage:"<<data;

    //数据包解析
    QStringList datalist=data.split("$$$$");
    QStringList list=datalist[0].split("#");

    //注册返回信息
    if(list[0]=="signon" && list[1]=="true"){
        QMessageBox::information(this,"congratulations","注册成功!",QMessageBox::Ok);
        QMessageBox::information(this,"congratulations","系统赠送给你了三个小精灵\n快去给他们起名字吧!",QMessageBox::Ok);
        player_data *myplayerdata=new player_data;
        myplayerdata->intoData(datalist[1].toStdString());//将用户数据灌入后，进欢迎界面
        this->hide();
        emit switchToWelcomePage(myplayerdata);
        disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&Widget::readMassage);
    }
    else if(list[0]=="signon" && list[1]=="false")
        QMessageBox::information(this,"sorry","注册失败,用户名已经被注册!",QMessageBox::Ok);

    //登陆返回信息
    else if(list[0]=="login" && list[1]=="true"){
        QMessageBox::information(this,"congratulations","登录成功!",QMessageBox::Ok);
        player_data *myplayerdata=new player_data;
        myplayerdata->intoData(datalist[1].toStdString());//将用户数据灌入后，进主界面
        this->hide();
        emit switchToHomePage(myplayerdata);
        disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&Widget::readMassage);
    }
    else if(list[0]=="login" && list[1]=="false")
        QMessageBox::information(this,"sorry","登录失败,用户名或密码错误!",QMessageBox::Ok);
    else
        return;

    //处理其他游戏信息；
}
