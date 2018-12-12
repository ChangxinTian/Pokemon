#include "player.h"

bool player_data::intoData(string data)
{
    //解析数据
    QString dataString=QString::fromStdString(data);
    QStringList datalist=dataString.split("***");

    //输入玩家数据
    QStringList playerString=datalist[0].split("#");
    id=playerString[1].toStdString();
    password=playerString[2].toStdString();
    status=playerString[3].toStdString();
    level=playerString[4].toInt();
    brithday=playerString[5].toStdString();
    achievement=playerString[6].toStdString();
    gamecount=playerString[7].toInt();
    wincount=playerString[8].toInt();

    //输入玩家的精灵数据
    pokemons.clear();
    pokemon_creator pc;
    for(int i=1;i+1<datalist.size();i++){
        QStringList pokemonString = datalist[i].split("#");
        pokemon *tempPoke = pc.getPokemon(pokemonString[2].toStdString(),pokemonString[13].toStdString()\
                ,pokemonString[6].toStdString(),pokemonString[11].toInt());
        tempPoke->setName(pokemonString[1].toStdString());
        tempPoke->setID(pokemonString[2].toStdString());
        tempPoke->setStatus(pokemonString[3].toStdString());
        tempPoke->setAttack(pokemonString[4].toInt());
        tempPoke->setDefensive(pokemonString[5].toInt());
        tempPoke->setCharcter(pokemonString[6].toStdString());
        tempPoke->setCritiacalpoint(pokemonString[7].toInt());
        tempPoke->setExperience(pokemonString[8].toInt());
        tempPoke->setFulllife(pokemonString[9].toInt());
        tempPoke->setInterval(pokemonString[10].toInt());
        tempPoke->setLevel(pokemonString[11].toInt());
        tempPoke->setLife(pokemonString[12].toInt());
        tempPoke->setRace(pokemonString[13].toStdString());
        pokemons.push_back(*tempPoke);
    }
    return true;
}
QString player_data::outData()
{
    //构造玩家数据部分
    QString ret = "player#";
    ret+=QString::fromStdString(id)+"#";//id
    ret+=QString::fromStdString(password)+"#";//密码
    ret+=QString::fromStdString(status)+"#";//在线状态
    ret+=QString::number(level)+"#";//等级
    ret+=QString::fromStdString(brithday)+"#";//生日
    ret+=QString::fromStdString(achievement)+"#";//成就
    ret+=QString::number(gamecount)+"#";
    ret+=QString::number(wincount);

    ret+="***";

    //构造精灵数据部分
    for(unsigned int i=0;i<pokemons.size();i++){
        ret+="pokemon#";
        ret+=QString::fromStdString(pokemons[i].getName())+"#";//name
        ret+=QString::fromStdString(pokemons[i].getID())+"#";//id
        ret+=QString::fromStdString(pokemons[i].getStatus())+"#";//status
        ret+=QString::number(pokemons[i].getAttack())+"#";//attack
        ret+=QString::number(pokemons[i].getDefensive())+"#";//defensive
        ret+=QString::fromStdString(pokemons[i].getCharcter())+"#";//character
        ret+=QString::number(pokemons[i].getCritiacalpoint())+"#";//critiacalpoint
        ret+=QString::number(pokemons[i].getExperience())+"#";//experience
        ret+=QString::number(pokemons[i].getFulllife())+"#";//fulllife
        ret+=QString::number(pokemons[i].getInterval())+"#";//inter
        ret+=QString::number(pokemons[i].getLevel())+"#";//level
        ret+=QString::number(pokemons[i].getLife())+"#";//life
        ret+=QString::fromStdString(pokemons[i].getRace());//race
        ret+="***";
    }

    return ret;
}
