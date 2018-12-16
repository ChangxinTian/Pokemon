#include "choicepage.h"
#include "ui_choicepage.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QTableView>
#include <QColor>
#include <QMessageBox>
#include <algorithm>

choicePage::choicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choicePage)
{
    ui->setupUi(this);
}

choicePage::~choicePage()
{
    delete ui;
}

choicePage::choicePage(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choicePage)
{
    opponentdata=new player_data;
    ui->setupUi(this);
    this->setWindowTitle("World");;
    setWindowIcon(QIcon(":/resource/icon.png"));
    mysocket=s;
}
void choicePage::receiveSwitch(player_data *pd,int num)
{
    gameclass=num;
    myplayerdata=pd;
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&choicePage::readMassage);

    //向服务器索取对战精灵表
    QString data="opponent";
    mysocket->tcpsocket->write(data.toLatin1());

    //设置自己的精灵表
    setPokemonTable();
    this->show();
}
void choicePage::readMassage()
{
    QString data=mysocket->tcpsocket->readAll();
    qDebug()<<"choice:"<<data;

    //数据包解析
    QStringList datalist=data.split("$$$$");
    QStringList list=datalist[0].split("#");

    //获取到对战的精灵表
    if(list[0]=="opponent" && list[1]=="true"){
        opponentdata->intoData(datalist[1].toStdString());
        sort(opponentdata->pokemons.begin(),opponentdata->pokemons.end()\
             ,[](pokemon a, pokemon b) -> bool { return a.getLevel() < b.getLevel(); });
        //设置对战的精灵表并更新页面
        setOpponentTable();
        update();
    }
    return;
}

void choicePage::setOpponentTable()//设置对手的精灵表
{
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal, "名字");
    model->setHeaderData(1,Qt::Horizontal, "等级");
    model->setHeaderData(2,Qt::Horizontal, "种族");
    model->setHeaderData(3,Qt::Horizontal, "属性");

    for(unsigned i=0;i<this->opponentdata->pokemons.size();i++){
        model->setItem(int(i), 0, new QStandardItem(QString::fromStdString(this->opponentdata->pokemons[i].getName())));
        model->setItem(int(i), 1, new QStandardItem(QString::number(this->opponentdata->pokemons[i].getLevel())));
        model->setItem(int(i), 2, new QStandardItem(QString::fromStdString(this->opponentdata->pokemons[i].getRace())));
        model->setItem(int(i), 3, new QStandardItem(QString::fromStdString(this->opponentdata->pokemons[i].getCharcter())));
    }

    this->ui->opponentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->opponentTable->setModel(model);
    this->ui->spinBox_2->setMaximum(opponentdata->pokemons.size());
    this->ui->spinBox_2->setMinimum(1);
}

void choicePage::setPokemonTable()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal, "名字");
    model->setHeaderData(1,Qt::Horizontal, "等级");
    model->setHeaderData(2,Qt::Horizontal, "种族");
    model->setHeaderData(3,Qt::Horizontal, "属性");

    for(unsigned i=0;i<this->myplayerdata->pokemons.size();i++){
        model->setItem(int(i), 0, new QStandardItem(QString::fromStdString(this->myplayerdata->pokemons[i].getName())));
        model->setItem(int(i), 1, new QStandardItem(QString::number(this->myplayerdata->pokemons[i].getLevel())));
        model->setItem(int(i), 2, new QStandardItem(QString::fromStdString(this->myplayerdata->pokemons[i].getRace())));
        model->setItem(int(i), 3, new QStandardItem(QString::fromStdString(this->myplayerdata->pokemons[i].getCharcter())));
    }

    this->ui->pokemonTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->pokemonTable->setModel(model);
    this->ui->spinBox->setMaximum(myplayerdata->pokemons.size());
    this->ui->spinBox->setMinimum(1);
}

void choicePage::on_backButton_clicked()//返回主界面
{
    emit switchToHomePage(myplayerdata);
    disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&choicePage::readMassage);
    this->hide();
}

void choicePage::on_okButton_clicked()//确认选择
{
    int mynum=ui->spinBox->value();
    int opnum=ui->spinBox_2->value();
    qDebug()<<mynum<<"<->"<<opnum;

    //检查非法输入
    if(mynum<1||opnum<1){
        ui->spinBox->cleanText();
        ui->spinBox_2->cleanText();
        QMessageBox::information(this,"Error",QString::fromStdString("输入编号有误！"));
    }

    //判断不同的游戏类型
    if(gameclass==1){//娱乐赛
        emit switchToBattlePage(&myplayerdata->pokemons[mynum-1],&opponentdata->pokemons[opnum-1]);
    }
    else {//升级或决斗赛
        emit switchToAutobattlePage(&myplayerdata->pokemons[mynum-1],&opponentdata->pokemons[opnum-1],gameclass,mynum-1);
    }

    disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&choicePage::readMassage);
    this->hide();
}
