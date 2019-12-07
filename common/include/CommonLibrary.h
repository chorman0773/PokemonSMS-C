#ifndef COMMON_COMMONLIBRARY_H_2019_12_06_18_34
#define COMMON_COMMONLIBRARY_H_2019_12_06_18_34

#ifdef __cplusplus
extern"C"{
#endif

#ifndef _WIN32
#define POKEMONSMS_COMMON_EXPORT __attribute__((visibility("default")))
#define POKEMONSMS_COMMON_IMPORT
#else
#define POKEMONSMS_COMMON_EXPORT __declspec(dllexport)
#define POKEMONSMS_COMMON_IMPOPT __declspec(dllimport)
#endif
#ifdef POKEMONSMS_COMMON_BUILD
#define POKEMONSMS_COMMON_API POKEMONSMS_COMMON_EXPORT
#else
#define POKEMONSMS_COMMON_API POKEMONSMS_COMMOM_IMPORT
#endif

#include <lua.h>

POKEMONSMS_COMMON_API void PokemonSMS_Common_OnInit(lua_State* rootState);

POKEMONSMS_COMMON_API void PokemonSMS_Common_Load(lua_State* rootState);

#ifdef __cplusplus
};
#endif

#endif
