#include "pokemon.h"

pokemon* pokemon_creator::getPokemon(string id,string race, string chara,int level)
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
    newpet->setID(id);
    newpet->setCharcter(chara);
    newpet->setLevel(level);

    return newpet;
}
