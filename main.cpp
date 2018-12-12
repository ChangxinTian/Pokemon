#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "player.h"
#include "pokemon.h"
#include "relatedfunc.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug()<<"\n测试精灵生成函数和参数设置函数\n";

    pokemon_creator pc;
    pokemon *p1=pc.getPokemon("test","METAL","FORCE",1);
    pokemon *p2=pc.getPokemon("test","WOOD","ENDURANCE",1);

    p1->setName("p1");
    p2->setName("p2");

    p1->ShowData();
    p2->ShowData();

    qDebug()<<"\n测试普通攻击函数\n";

    p1->Attack(p2);
    p1->ShowData();

    p2->Attack(p1);
    p2->ShowData();

    qDebug()<<"\n测试技能攻击函数\n";

    p1->SkillAttack(p2);
    p1->ShowData();

    p2->SkillAttack(p1);
    p2->ShowData();

    qDebug()<<"\n测试升级函数\n";

    p1->addExperience(200);

    p2->addExperience(200);

    return a.exec();
}


