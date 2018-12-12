#include "housepage.h"
#include "ui_housepage.h"

#include <QStandardItemModel>

housePage::housePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::housePage)
{
    ui->setupUi(this);
}

housePage::~housePage()
{
    delete ui;
}

void housePage::receiveSwitch(player_data *pd)
{
    myplayerdata=pd;

    //设置表单格式
    this->ui->tableView->setAlternatingRowColors(true);

    //设置模板格式
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(10);
    model->setHeaderData(0,Qt::Horizontal, "名字");
    model->setHeaderData(1,Qt::Horizontal, "种族");
    model->setHeaderData(2,Qt::Horizontal, "属性");
    model->setHeaderData(3,Qt::Horizontal, "等级");
    model->setHeaderData(4,Qt::Horizontal, "生命");
    model->setHeaderData(5,Qt::Horizontal, "攻击");
    model->setHeaderData(6,Qt::Horizontal, "防御");
    model->setHeaderData(7,Qt::Horizontal, "暴击");
    model->setHeaderData(8,Qt::Horizontal, "经验");
    model->setHeaderData(9,Qt::Horizontal, "攻击间隔");

    //用玩家数据填充模板
    int row=0;
    for(unsigned int i=0;i<pd->pokemons.size();i++){
        model->setItem(row, 0, new QStandardItem(QString::fromStdString(pd->pokemons[i].getName())));
        model->setItem(row, 1, new QStandardItem(QString::fromStdString(pd->pokemons[i].getRace())));
        model->setItem(row, 2, new QStandardItem(QString::fromStdString(pd->pokemons[i].getCharcter())));
        model->setItem(row, 3, new QStandardItem(QString::number(pd->pokemons[i].getLevel())));
        model->setItem(row, 4, new QStandardItem(QString::number(pd->pokemons[i].getLife())));
        model->setItem(row, 5, new QStandardItem(QString::number(pd->pokemons[i].getAttack())));
        model->setItem(row, 6, new QStandardItem(QString::number(pd->pokemons[i].getDefensive())));
        model->setItem(row, 7, new QStandardItem(QString::number(pd->pokemons[i].getCritiacalpoint())));
        model->setItem(row, 8, new QStandardItem(QString::number(pd->pokemons[i].getExperience())));
        model->setItem(row, 9, new QStandardItem(QString::number(pd->pokemons[i].getInterval())));
        row++;
    }

    //将模板置到表单
    this->ui->tableView->setModel(model);
    this->show();
}

void housePage::on_pushButton_clicked()//返回主界面
{
    emit switchToHomePage(myplayerdata);
    this->hide();
}
