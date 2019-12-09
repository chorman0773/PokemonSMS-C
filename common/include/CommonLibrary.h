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
#include <stdint.h>

POKEMONSMS_COMMON_API void PokemonSMS_Common_OnInit(lua_State* state);
POKEMONSMS_COMMON_API void PokemonSMS_Common_Register(lua_State* state);

POKEMONSMS_COMMON_API void PokemonSMS_Common_Load();

POKEMONSMS_COMMON_API void PokemonSMS_Common_Close();

POKEMONSMS_COMMON_API pthread_cond_t* PokemonSMS_Common_Dispatch(void(*callback)(void*,uint64_t,lua_State*),void* handle,void(*handle_dtor)(void*));

#ifdef __cplusplus
};
#endif

#endif
