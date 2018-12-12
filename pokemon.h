#ifndef POKEMON_H
#define POKEMON_H

#include "relatedfunc.h"

//影响升级策略
//力量型：高攻击;肉盾型：高生命值;防御型：高防御;敏捷型：低攻击间隔
enum Character{FORCE,ENDURANCE,DEFENSE,AGILE};

//影响不同的技能攻击
//种族，金木水火土，对应不同的特殊攻击
enum Race {METAL,WOOD,WATER, FIRE, EARTH};

//控制升级时参数升级幅度
const int UpGradeValue = 10;

class poke_data{
public:
    string name;//名字
    string ID;//宠物ID
    string character;//属性，四种之一
    string race;//种族，五种之一
    int life;//生命值
    int full_life;//当前等级最大生命值
    int level;//等级 1-15
    int experience;//当前经验值
    int attack;//攻击
    int defensive;//防御力
    int critical_point; //暴击指数
    int interval;//攻击间隔
};

class pokemon
{
private:
    string name;//名字
    string ID;//宠物ID
    string character;//属性，四种之一
    string race;//种族，五种之一
    string status;//状态
    int life;//生命值
    int full_life;//当前等级最大生命值
    int level;//等级 1-15
    int experience;//当前经验值
    int attack;//攻击
    int defensive;//防御力
    int critical_point; //暴击指数
    int interval;//攻击间隔

public:
    pokemon(){
        ID="";
        name = "NULL";
        level = 1;
        experience = 0;
        life = full_life=120;
        attack = 30;
        defensive = 15;
        interval=7;
        critical_point=10;
        status="live";
    }
    pokemon(pokemon *p){
        ID=p->getID();
        name =p->getName();
        level =p->getLevel();
        experience = p->getExperience();
        life = full_life=p->getLife();
        attack = p->getAttack();
        defensive = p->getDefensive();
        interval=p->getInterval();
        critical_point=p->getCritiacalpoint();
        status=p->getStatus();
    }
    virtual ~pokemon();

    bool operator<(const pokemon &c)const {return this->level<c.level;}
    void setName(string n) {name=n;}
    void setID(string i) {ID=i;}
    void setCharcter(string c) {character=c;}
    void setRace(string r) {race=r;}
    void setLevel(int l) {level=l;}
    void setLife(int l) {life=l;}
    void setFulllife(int f) {full_life=f;}
    void setExperience(int e) {experience=e;}
    void setAttack(int a) {attack=a;}
    void setDefensive(int d) {defensive=d;}
    void setCritiacalpoint(int c) {critical_point=c;}
    void setInterval(int i) {interval=i; if(interval<2&&level<11) interval=2;}
    void setStatus(string s) {status=s;}

    string getName() {return name;}
    string getID() {return ID;}
    string getCharcter() {return character;}
    int getLevel() {return level;}
    int getLife() {return life;}
    int getFulllife() {return full_life;}
    int getExperience() {return experience;}
    int getAttack() {return attack;}
    int getDefensive() {return defensive;}
    int getCritiacalpoint() {return critical_point;}
    int getInterval() {return interval;}
    string getRace() {return race;}
    string getStatus(){return status;}

    bool addExperience(int exp);//加经验
    void reStart();//重生
    void ShowData();//展示各项数据
    bool Upgrade();//升级函数
    int Attack(pokemon *op);//攻击函数
    int randStrike();//暴击函数

    virtual int SkillAttack(pokemon *op);//攻击方式
    virtual void ShowDataExtra(void) {}//展示特征性数据
};

class pokemonMetal : public pokemon
{
public:
    pokemonMetal() {setRace("METAL");}
    pokemonMetal(poke_data data);
    ~pokemonMetal() {}
    void ShowDataExtra(void) {	cout << "race:" << "Metal" << "\t\t" << endl;}
    int SkillAttack(pokemon *op); //直接造成额外真实伤害
};

class pokemonWood : public pokemon
{
public:
    pokemonWood() {setRace("WOOD");}
    pokemonWood(poke_data data);
    ~pokemonWood() {}
    void ShowDataExtra(void) {	cout << "race:" << "Wood" << "\t\t" << endl;}
    int SkillAttack(pokemon *op);//回血百分之二十
};

class pokemonWater : public pokemon
{
public:
    pokemonWater() {setRace("WATER");}
    pokemonWater(poke_data data);
    ~pokemonWater() {}
    void ShowDataExtra(void) {	cout << "race:" << "Water" << "\t\t" << endl;}
    int SkillAttack(pokemon *op);//在本轮中减少一个攻击间隔
};

class pokemonFire : public pokemon
{
public:
    pokemonFire() {setRace("FIRE");}
    pokemonFire(poke_data data);
    ~pokemonFire() {}
    void ShowDataExtra(void) {	cout << "race:" << "Fire" << "\t\t" << endl;}
    int SkillAttack(pokemon *op);//在本轮增加百分之十攻击力
};

class pokemonEarth : public pokemon
{
public:
    pokemonEarth() { setRace("EARTH"); }
    pokemonEarth(poke_data data);
    ~pokemonEarth() {}
    void ShowDataExtra(void) {	cout << "race:" << "Earth" << "\t\t" << endl;}
    int SkillAttack(pokemon *op);//在本轮增加百分之十防御力
};

//精灵生成类
class pokemon_creator
{
public:
    pokemon_creator(){}
    ~pokemon_creator(){}
    pokemon *getPokemon(string newId,string race,string newCharater,int level);
    pokemon *getPokemon(string race);
    pokemon *getPokemon(pokemon *poke);
};


#endif // POKEMON_H
