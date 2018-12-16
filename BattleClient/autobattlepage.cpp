#include "autobattlepage.h"
#include "ui_autobattlepage.h"

#include <QImage>
#include <QTime>
#include "relatedfunc_client.h"

autobattlePage::autobattlePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autobattlePage)
{
    ui->setupUi(this);
}

autobattlePage::~autobattlePage()
{
    delete ui;
}

autobattlePage::autobattlePage(socket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autobattlePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Battle");
    setWindowIcon(QIcon(":/resource/icon.png"));
    mysocket=s;
    myattack = new QLabel(this);
    opattack = new QLabel(this);
}

void autobattlePage::receiveSwitch(pokemon *my,pokemon *op,int num,int mypokemonnum)
{
    gameclass=num;
    pokemonid=mypokemonnum;

    qDebug()<<"autoBattlePage";

    myPokemonSave=my;
    opPokemonSave=op;

    pokemon_creator pc;
    myPokemon=pc.getPokemon(my);
    opPokemon=pc.getPokemon(op);

    setImage();

    this->timer=new QTimer;
    connect(timer, &QTimer::timeout, this,&autobattlePage::battle);
    timer->start(1000);
    connect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&autobattlePage::readMassage);
    dataShow();

    this->show();
    void battle();
}

void autobattlePage::setImage()
{
    ui->progressBar1->setValue(100);
    ui->progressBar2->setValue(100);

    //载入己方精灵图片
    QImage image1;
    QString race1=QString::fromStdString(myPokemon->getRace());
    if(race1=="METAL")
        image1.load(":/resource/Bulbasaur1.png");
    else if(race1=="WOOD")
        image1.load(":/resource/Charmander1.png");
    else if(race1=="WATER")
        image1.load(":/resource/Squirtle1.png");
    else if(race1=="FIRE")
        image1.load(":/resource/water.png");
    else
        image1.load(":/resource/pkq.png");

    QImage mirroredImage = image1.mirrored(true, false);
    QPixmap myimage=QPixmap::fromImage(mirroredImage);
    myattack->setPixmap(myimage);
    myattack->setGeometry(150,350,100,150);
    myattack->setScaledContents(true);

    //载入对方精灵图片
    QImage image2;
    QString race2=QString::fromStdString(opPokemon->getRace());
    if(race2=="METAL")
        image2.load(":/resource/Bulbasaur1.png");
    else if(race2=="WOOD")
        image2.load(":/resource/Charmander1.png");
    else if(race2=="WATER")
        image2.load(":/resource/Squirtle1.png");
    else if(race2=="FIRE")
        image2.load(":/resource/water.png");
    else
        image2.load(":/resource/pkq.png");

    QPixmap opimage=QPixmap::fromImage(image2);
    opattack->setPixmap(opimage);
    opattack->setGeometry(550,350,100,150);
    opattack->setScaledContents(true);
}

void autobattlePage::dataShow()
{
    //展示己方数据
    this->ui->myData->setStyleSheet("QTextBrowser{border-width:0;border-style:outset;background-color:rgb(255,132,139,0);border-radius:3px;color:rgb(0,0,0);}");
    this->ui->myData->clear();
    this->ui->myData->append(QString::fromStdString(myPokemon->getName()));
    this->ui->myData->append(" ");
    this->ui->myData->append("\t种族:\t"+QString::fromStdString(myPokemon->getRace()));
    this->ui->myData->append("\t属性:\t"+QString::fromStdString(myPokemon->getCharcter()));
    this->ui->myData->append("\t生命值:\t"+QString::number(myPokemon->getLife()));
    this->ui->myData->append("\t攻击力:\t"+QString::number(myPokemon->getAttack()));
    this->ui->myData->append("\t防御力:\t"+QString::number(myPokemon->getDefensive()));
    this->ui->myData->append("\t暴击点:\t"+QString::number(myPokemon->getCritiacalpoint()));

    //展示对方数据
    this->ui->opData->setStyleSheet("QTextBrowser{border-width:0;border-style:outset;background-color:rgb(255,132,139,0);border-radius:3px;color:rgb(0,0,0);}");
    this->ui->opData->clear();
    this->ui->opData->append(QString::fromStdString(opPokemon->getName()));
    this->ui->opData->append(" ");
    this->ui->opData->append("\t种族:\t"+QString::fromStdString(opPokemon->getRace()));
    this->ui->opData->append("\t属性:\t"+QString::fromStdString(opPokemon->getCharcter()));
    this->ui->opData->append("\t生命值:\t"+QString::number(opPokemon->getLife()));
    this->ui->opData->append("\t攻击力:\t"+QString::number(opPokemon->getAttack()));
    this->ui->opData->append("\t防御力:\t"+QString::number(opPokemon->getDefensive()));
    this->ui->opData->append("\t暴击点:\t"+QString::number(opPokemon->getCritiacalpoint()));
}

void autobattlePage::paintEvent(QPaintEvent *)
{
    /* 此处设置背景
    QPainter p(this);
    p.drawPixmap(0,0,width(),height(),QPixmap(":/resource/background3.jpg"));
    p.end();
    */

    dataShow();
}

void autobattlePage::battle()
{
    ui->status->clear();

    static int mytime=myPokemon->getInterval()/2;
    static int optime=opPokemon->getInterval();

    //攻击间隔检查，如果已到时间，则进行攻击
    if(myPokemon->getLife()>0&&opPokemon->getLife()>0){
        mytime--;
        if(mytime==0){
            if(randNum()<3){
                int ds=myPokemon->SkillAttack(opPokemon);
                ui->status->setText(QString("己方发动技能攻击！造成%1点伤害").arg(ds));
            }
            else{
                int ds=myPokemon->Attack(opPokemon);
                ui->status->setText(QString("己方发动普通攻击！造成%1点伤害").arg(ds));
            }
            mytime=myPokemon->getInterval();  
        }
        else if(mytime==1){
            showAttackOp();
        }
        update();
        if(myPokemon->getLife()>0&&opPokemon->getLife()>0){
            optime--;
            if(optime==0){
                if(randNum()<3){
                    int ds=opPokemon->SkillAttack(myPokemon);
                    ui->status->setText(QString("对方发动技能攻击！造成%1点伤害").arg(ds));
                }
                else{
                    int ds=opPokemon->Attack(myPokemon);
                    ui->status->setText(QString("对方发动普通攻击！造成%1点伤害").arg(ds));
                }
                optime=opPokemon->getInterval();
            }
            else if(optime==1){
                showAttackMe();
            }

        }

        ui->lcdNumber->display(mytime-1);
        ui->lcdNumber_2->display(optime-1);

        ui->progressBar1->setValue((myPokemon->getLife()*100)/myPokemon->getFulllife());
        ui->progressBar2->setValue((opPokemon->getLife()*100)/opPokemon->getFulllife());

        update();
        return;
    }

    timer->stop();
    timer->deleteLater();

    QString sendData;
    if(myPokemon->getLife()>0)
        sendData="win";
    else
        sendData="lose";

    emit sendBattleResult(sendData,opPokemonSave,gameclass,pokemonid);

    disconnect(mysocket->tcpsocket,&QTcpSocket::readyRead,this,&autobattlePage::readMassage);
    this->hide();
}

void autobattlePage::showAttackOp()//攻击对方的动画
{
    QPropertyAnimation* animation = new QPropertyAnimation(myattack, "geometry");
    animation->setDuration(1000);
    animation->setKeyValueAt(0,QRect(150,350,100,150));
    animation->setKeyValueAt(0.4,QRect(500,350, 100, 150));
    animation->setKeyValueAt(1,QRect(150,350,100,150));
    animation->setEasingCurve(QEasingCurve::InBack);
    animation->setLoopCount(1);
    animation->start();
}

void autobattlePage::showAttackMe()//攻击我的动画
{
    QPropertyAnimation* animation = new QPropertyAnimation(opattack, "geometry");
    animation->setDuration(1000);
    animation->setKeyValueAt(0,QRect(550,350, 100, 150));
    animation->setKeyValueAt(0.4,QRect(200,350, 100, 150));
    animation->setKeyValueAt(1,QRect(550,350, 100, 150));
    animation->setEasingCurve(QEasingCurve::InBack);
    animation->setLoopCount(1);
    animation->start();
}

void autobattlePage::readMassage()//获取tcp数据并输出
{
    QString data=mysocket->tcpsocket->readAll();
    qDebug()<<"choice:"<<data;
    return;
}



