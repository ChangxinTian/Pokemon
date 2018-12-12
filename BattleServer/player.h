#ifndef PLAYER_H
#define PLAYER_H

#include"pokemon.h"


class player_data{
public:
    string id;//用户ID
    string password;//用户密码
    string status;//状态（在线，下线，注销）
    int level;//用户等级
    string brithday;//生日
    string achievement;//成就（勋章）
    int gamecount;//游戏场次计数
    int wincount;//胜利场次计数
    vector<pokemon> pokemons;//精灵vector

    bool intoData(string data);//用字符串灌入数据
    QString outData();//通过字符串输出数据
    bool noviceGift(QString newId,QString newPassword);
    bool gift();
};

class player
{
private:
    string id;//用户ID
    string password;//用户密码
    string status;//状态（在线，下线，注销）
    int level;//用户等级
    string brithday;//生日
    string achievement;//成就（勋章）
    int gamecount;//游戏场次计数
    int wincount;//胜利场次计数
    vector<pokemon> pokemons;//精灵vector
public:
    player(){
        id = "NULL";
        password = "NULL";
        level = 0; 
        gamecount=0;
        wincount=0;
        brithday = QDateTime::currentDateTime().toString().toStdString();
        achievement="00";
        pokemons.clear();
    }
    player(string id,string password){
        id=id;
        password = password;
        level=0;
        gamecount=0;
        wincount=0;
        brithday = QDateTime::currentDateTime().toString().toStdString();
        achievement="00";
        pokemons.clear();
    }
    player(player_data data){
        id=data.id;
        password=data.password;
        level=data.level;
        status=data.status;
        brithday=data.brithday;
        achievement=data.achievement;
        gamecount=data.gamecount;
        wincount=data.wincount;
        pokemons.clear();
        pokemons.assign(data.pokemons.begin(), data.pokemons.end());
    }
    ~player();
    
    void setId(string i) {id=i;}
    void setPassword(string p) {password=p;}
    void setLevel(int l){level=l;}
    void setBrithday(string b) {brithday=b;}
    void setGamecount(int g){gamecount=g;}
    void setWincount(int w){wincount=w;}
    void addPokemon(pokemon newpet) {pokemons.push_back(newpet);}
    bool setAchievement(string ach){
        if(ach.length()!=15)
            return false;
        achievement=ach;
        return true;
    }
    void setData(player_data pd){
        id=pd.id;
        password=pd.password;
        level=pd.level;
        status=pd.status;
        brithday=pd.brithday;
        pokemons.clear();
        pokemons.assign(pd.pokemons.begin(), pd.pokemons.end());
    }

    string getId() {return id;}
    string getPassword(){return password;}
    int getPokemonNum() {return int(pokemons.size());}
    int getLevel() {return level;}
    int getGamecount() {return gamecount;}
    int getWincount() {return wincount;}
    string getBrithday() {return brithday;}
    string getAchievement(){return achievement;}
    vector<pokemon> getPokemons() {return pokemons;}
    void getData(player_data *pd){
        pd->brithday=brithday;
        pd->id=id;
        pd->level=level;
        pd->password=password;
        pd->pokemons.assign(pokemons.begin(),pokemons.end());
    }
};

#endif // PLAYER_H
