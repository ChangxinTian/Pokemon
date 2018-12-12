#include "worldpage.h"
#include "ui_worldpage.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QTableView>
#include <QColor>

worldPage::worldPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::worldPage)
{
    ui->setupUi(this);
}
worldPage::worldPage(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::worldPage)
{
    ui->setupUi(this);
    this->setWindowTitle("World");
    setWindowIcon(QIcon(":/resource/icon.png"));
    mysocket=s;
}
worldPage::~worldPage()
{
    delete ui;
}
void worldPage::receiveSwitch(player_data *pd)
{
    myplayerdata=pd;
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&worldPage::readMassage);
    QString data="worldlist";
    mysocket->tcpsocket->write(data.toLatin1());//向服务器发出申请，获取在线用户表和精灵表
    this->show();
}

void worldPage::on_pushButton_clicked()
{
    emit switchToHomePage(myplayerdata);
    disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&worldPage::readMassage);

    this->hide();
}
void worldPage::readMassage()
{
    QString data=mysocket->tcpsocket->readAll();

    qDebug()<<"\nworldPage:"<<data;

    QStringList datalist=data.split("$$$$");
    QStringList list=datalist[0].split("#");

    //分析数据包，设置用户表和精灵表
    if(list[0]=="worldlist" && list[1]=="true"){
        setUserTable(datalist[1]);
        setPokemonTable(datalist[2]);
    }

    return;
}
void worldPage::setUserTable(QString data)
{
    //设置表单格式
    this->ui->userTable->resizeColumnsToContents();
    this->ui->userTable->setAlternatingRowColors(true);

    //设置表单模板
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal, "ID");
    model->setHeaderData(1,Qt::Horizontal, "等级");
    model->setHeaderData(2,Qt::Horizontal, "生日");
    model->setHeaderData(3,Qt::Horizontal, "胜率");

    //向模板中灌入数据
    QStringList item1=data.split("***");
    for(int i=0;i+1<item1.size();i++){
        QStringList item2=item1[i].split("#");
        model->setItem(i, 0, new QStandardItem(item2[0]));
        model->setItem(i, 1, new QStandardItem(item2[1]));
        model->setItem(i, 2, new QStandardItem(item2[2]));
        int gcount=item2[4].toInt();
        int wcount=item2[5].toInt();
        if(gcount==0)
            model->setItem(i, 3, new QStandardItem("0"));
        else{
            double winningProbability= double(wcount)/double(gcount);
            model->setItem(i, 3, new QStandardItem(QString::number(winningProbability)));
        }
    }

    //将模板设置到表单中
    this->ui->userTable->setModel(model);
}
void worldPage::setPokemonTable(QString data)
{
    //设置表单格式
    this->ui->pokemonTable->resizeColumnsToContents();
    this->ui->pokemonTable->setAlternatingRowColors(true);

    //设置表单模板
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal, "名字");
    model->setHeaderData(1,Qt::Horizontal, "主人");
    model->setHeaderData(2,Qt::Horizontal, "种族");
    model->setHeaderData(3,Qt::Horizontal, "等级");

    //向模板中灌入数据
    QStringList item1=data.split("***");
    for(int i=0;i+1<item1.size();i++){
        QStringList item2=item1[i].split("#");
        model->setItem(i, 0, new QStandardItem(item2[0]));
        model->setItem(i, 1, new QStandardItem(item2[1]));
        model->setItem(i, 2, new QStandardItem(item2[3]));
        model->setItem(i, 3, new QStandardItem(item2[2]));
    }

    //将模板设置到表单中
    this->ui->pokemonTable->setModel(model);
}
