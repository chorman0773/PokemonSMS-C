#include <CommonLibrary.h>
#include <lua.h>
#include <Pokemon.h>
#include <stdio.h>

void PokemonSMS_Common_OnInit(lua_State* rootState){
    puts("pokemonsms-common: OnInit");
}

void PokemonSMS_Common_Load(lua_State* rootState){
    puts("pokemonsms-common: Load");
}
