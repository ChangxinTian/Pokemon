#include "deletepage.h"
#include "ui_deletepage.h"
#include "relatedfunc_client.h"
#include <QMessageBox>

deletePage::deletePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deletePage)
{
    ui->setupUi(this);
}
deletePage::deletePage(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deletePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Delete");
    setWindowIcon(QIcon(":/resource/icon.png"));
    mysocket=s;
}
deletePage::~deletePage()
{
    delete ui;
}

void deletePage::receiveSwitch(player_data *pd)
{
    myplayerdata=pd;
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&deletePage::readMassage);
    ui->spinBox->setMaximum(3);
    ui->spinBox->setMinimum(1);

    //随机产生3个精灵，供玩家选择抛弃
    unsigned int s=myplayerdata->pokemons.size();
    n=randNum()%s;//返回0-9的随机数
    ui->label1->setText(QString::fromStdString(myplayerdata->pokemons[n].getName()));
    ui->label2->setText(QString::fromStdString(myplayerdata->pokemons[(n+1)%s].getName()));
    ui->label3->setText(QString::fromStdString(myplayerdata->pokemons[(n+2)%s].getName()));

    this->show();
}

void deletePage::on_pushButton_clicked()
{
    int i=ui->spinBox->value();

    //确认选择，改变精灵状态为"dead"
    i=(n+i-1)/myplayerdata->pokemons.size();
    myplayerdata->pokemons[i].setStatus("dead");

    //向服务器更新信息
    QString data=myplayerdata->outData();
    data="delete$$$$"+data;
    mysocket->tcpsocket->write(data.toLatin1());
}

void deletePage::readMassage()
{
    QString data=mysocket->tcpsocket->readAll();
    qDebug()<<"\ndeletePage:"<<data;

    //解析数据包
    QStringList datalist=data.split("$$$$");
    QStringList list=datalist[0].split("#");

    //更新本地信息（删除了精灵之后的），并返回主界面
    if(list[0]=="delete" && list[1]=="true"){
        QMessageBox::information(this,"ok",QString::fromStdString("成功抛弃！"));
        myplayerdata->intoData(datalist[1].toStdString());
        disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&deletePage::readMassage);
        emit switchToHomePage(myplayerdata);
        this->hide();
    }
}
