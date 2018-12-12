#include "pokemon.h"

pokemon::~pokemon()
{}

int pokemon::SkillAttack(pokemon *op)//基类技能攻击虚函数
{
    int d;
    d=this->Attack(op);
    return d;
}

void pokemon::reStart()//重生函数
{
    level = 1;
    experience = 0;
    life = full_life=120;
    attack = 30;
    defensive = 15;
    critical_point=10;
    interval= 7;
}
void pokemon::ShowData()//打印信息
{
    cout << "------------------------------------------" << endl;
    cout << "ID:" << ID << "\t\t" << ends;
    cout << "name:" << name << "\t\t" << ends;
    cout << "level:" << level << "\t\t" << ends;
    cout << "exp:" << experience << "\t\t" << ends;
    cout << "life:" << life << "\t\t" << ends;
    cout << "attack:" << attack << "\t\t" << ends;
    cout << "defense:" << defensive << "\t\t" << ends;
    ShowDataExtra();
    cout << "-------------------------------------------" << endl;

}

int pokemon::Attack(pokemon *op)//攻击函数
{
    int demage=0;

    if(getAttack()-(op->getDefensive()/2)>0)
        demage=getAttack()-(op->getDefensive()/2)+randStrike();
    //当双方差距悬殊，避免攻击为负数
    else
        demage=getAttack()/10+randStrike();

    op->setLife(op->getLife()-demage);//造成攻击

    return demage;
}

int pokemon::randStrike()//随机暴击函数
{
    if(randNum()<2)
        return getCritiacalpoint();
    return 0;
}

bool pokemon::addExperience(int exp)//加经验
{
    experience+=exp;
    return Upgrade();
}

bool pokemon::Upgrade()//升级函数
{
    if (level >= 15)
        return false;
    if (experience < level * 100)
        return false;

    cout << name << "upgrade!!!" << endl;


    //升级参数
    life = life + level * UpGradeValue;
    attack = attack + level * UpGradeValue;
    defensive = defensive + level * UpGradeValue;

    //不同类型的特异性升级
    if("FORCE"==character){
        attack += UpGradeValue;
    }
    else if("ENDURANCE"==character){
        life += UpGradeValue;
    }
    else if("DEFENSE"==character){
        defensive += UpGradeValue;
    }
    else if("AGILE"==character){
        if(level%3)
            interval--;
    }
    else{
        cout<<"upgrade error!"<<endl;
    }

    experience=experience-level*100;
    level++;

    ShowData();//展示升级后数据

    return true;
}

pokemonMetal::pokemonMetal(poke_data data)
{
    this->setRace(data.race);
    this->setAttack(data.attack);
    this->setCharcter(data.character);
    this->setCritiacalpoint(data.critical_point);
    this->setDefensive(data.defensive);
    this->setExperience(data.experience);
    this->setFulllife(data.full_life);
    this->setID(data.ID);
    this->setInterval(data.interval);
    this->setLevel(data.level);
    this->setLife(data.life);
    this->setName(data.name);
}

int pokemonMetal::SkillAttack(pokemon *op)//直接造成额外真实伤害
{
    int  d=0;
    d=this->Attack(op);
    op->setLife(op->getLife()-getCritiacalpoint());
    return d+getCritiacalpoint();
}

pokemonWood::pokemonWood(poke_data data)
{
    this->setRace(data.race);
    this->setAttack(data.attack);
    this->setCharcter(data.character);
    this->setCritiacalpoint(data.critical_point);
    this->setDefensive(data.defensive);
    this->setExperience(data.experience);
    this->setFulllife(data.full_life);
    this->setID(data.ID);
    this->setInterval(data.interval);
    this->setLevel(data.level);
    this->setLife(data.life);
    this->setName(data.name);
}
int pokemonWood::SkillAttack(pokemon *op)//回血百分之二十
{
    int d=0;
    d=this->Attack(op);
    this->setLife((this->getLife()*12)/10);
    return d;
}

pokemonWater::pokemonWater(poke_data data)
{
    this->setRace(data.race);
    this->setAttack(data.attack);
    this->setCharcter(data.character);
    this->setCritiacalpoint(data.critical_point);
    this->setDefensive(data.defensive);
    this->setExperience(data.experience);
    this->setFulllife(data.full_life);
    this->setID(data.ID);
    this->setInterval(data.interval);
    this->setLevel(data.level);
    this->setLife(data.life);
    this->setName(data.name);
}
int pokemonWater::SkillAttack(pokemon *op)//在本轮中减少一个攻击间隔
{
    int d=0;
    d=this->Attack(op);
    this->setInterval(this->getInterval()-1);
    return d;
}

pokemonFire::pokemonFire(poke_data data)
{
    this->setRace(data.race);
    this->setAttack(data.attack);
    this->setCharcter(data.character);
    this->setCritiacalpoint(data.critical_point);
    this->setDefensive(data.defensive);
    this->setExperience(data.experience);
    this->setFulllife(data.full_life);
    this->setID(data.ID);
    this->setInterval(data.interval);
    this->setLevel(data.level);
    this->setLife(data.life);
    this->setName(data.name);
}
int pokemonFire::SkillAttack(pokemon *op)//在本轮增加百分之十攻击力
{
    int d;
    d=this->Attack(op);
    this->setAttack((this->getAttack()*11)/10);
    return d;
}

pokemonEarth::pokemonEarth(poke_data data)
{
    this->setRace(data.race);
    this->setAttack(data.attack);
    this->setCharcter(data.character);
    this->setCritiacalpoint(data.critical_point);
    this->setDefensive(data.defensive);
    this->setExperience(data.experience);
    this->setFulllife(data.full_life);
    this->setID(data.ID);
    this->setInterval(data.interval);
    this->setLevel(data.level);
    this->setLife(data.life);
    this->setName(data.name);
}
int pokemonEarth::SkillAttack(pokemon *op)//在本轮增加百分之十防御力
{
    int d;
    d=this->Attack(op);
    this->setDefensive((this->getDefensive()*11)/10);
    return d;
}

pokemon* pokemon_creator::getPokemon(string newId,string race,string newCharater,int level)
{
    pokemon *newpet;

    if(race=="METAL") newpet=new pokemonMetal;
    else if(race=="WOOD") newpet=new pokemonWood;
    else if(race=="WATER") newpet=new pokemonWater;
    else if(race=="FIRE") newpet=new pokemonFire;
    else if(race=="EARTH") newpet=new pokemonEarth;
    else {
        cout<<"getPokemon error"<<endl;
        newpet=new pokemon;
    }

    newpet->setCharcter(newCharater);
    newpet->setLevel(level);
    newpet->setID(newId);

    return newpet;
}

pokemon* pokemon_creator::getPokemon(string race)
{
    pokemon *newpet;

    if(race=="METAL") newpet=new pokemonMetal;
    else if(race=="WOOD") newpet=new pokemonWood;
    else if(race=="WATER") newpet=new pokemonWater;
    else if(race=="FIRE") newpet=new pokemonFire;
    else if(race=="EARTH") newpet=new pokemonEarth;
    else {
        cout<<"getPokemon error"<<endl;
        newpet=new pokemon;
    }
    return newpet;
}

pokemon *pokemon_creator::getPokemon(pokemon *poke)
{
    pokemon *newpet;

    if(poke->getRace()=="METAL") newpet=new pokemonMetal;
    else if(poke->getRace()=="WOOD") newpet=new pokemonWood;
    else if(poke->getRace()=="WATER") newpet=new pokemonWater;
    else if(poke->getRace()=="FIRE") newpet=new pokemonFire;
    else if(poke->getRace()=="EARTH") newpet=new pokemonEarth;
    else {
        cout<<"getPokemon error"<<endl;
        newpet=new pokemon;
    }

    newpet->setAttack(poke->getAttack());
    newpet->setCharcter(poke->getCharcter());
    newpet->setCritiacalpoint(poke->getCritiacalpoint());
    newpet->setDefensive(poke->getDefensive());
    newpet->setExperience(poke->getExperience());
    newpet->setFulllife(poke->getFulllife());
    newpet->setID(poke->getID());
    newpet->setInterval(poke->getInterval());
    newpet->setLevel(poke->getLevel());
    newpet->setLife(poke->getLife());
    newpet->setName(poke->getName());
    newpet->setRace(poke->getRace());
    newpet->setStatus(poke->getStatus());

    return newpet;
}
