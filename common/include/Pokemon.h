#ifdef COMMON_POKEMON_H_2019_12_05_18_05
#define COMMON_POKEMON_H_2019_12_05_18_05

#ifdef __cplusplus
extern"C"{
#endif
#include <CommonLibrary.h>
#include <stdint.h>

typedef struct Type Type;
typedef struct Move Move;
typedef struct Ability Ability;
typedef struct Form Form;
typedef struct PokemonSpecies PokemonSpecies;
typedef struct Item Item;
typedef struct Pokemon Pokemon;
typedef struct ItemStack ItemStack;

typedef uint32_t MoveCategory;
typedef uint32_t Stat;

enum{
    CATEGORY_PHYSICAL = 0,
    CATEGORY_SPECIAL  = 1,
    CATEGORY_STATUS   = 2
};

enum{
    STAT_ATTACK  = 0,
    STAT_DEFENSE = 1,
    STAT_SPECIAL = 2,
    STAT_SPECDEF = 3,
    STAT_SPEED   = 4,
    STAT_HP      = 5
};

typedef bool(*item_CanHold)(ItemStack*,Pokemon*);
typedef bool(*item_CanUseOn)(ItemStack*,Pokemon*);

typedef bool(*move_CanUse)(void*,Move*,Pokemon*);
typedef Type*(*move_GetType)(void*,Move*,Pokemon*);
typedef MoveCategory(*move_GetCategory)(void*,Move*,Pokemon*);

typedef Form*(*pokemon_GetForm)(void*,Pokemon*);

typedef void(*event_handler)(void*,uint32_t,...);

typedef uint32_t Field;

Type* Type_system_typeless();
Move* Move_null();
Ability* Ability_null();
Item* Item_null();
PokemonSpecies* Species_null();

enum{
    /* const char* */
    FIELD_LOCATION = 0,
    /* struct json_object * */
    FIELD_UNNAME = 1,
    /* struct json_object * */
    FIELD_DESCRIPTION = 2,
    /* event_handler */
    FIELD_BUS = 3,
    /* const char* */
    FIELD_TYPE_ADD_WEEKNESS = 4,
    /* const char* */
    FIELD_TYPE_ADD_RESISTANCE = 5,
    /* const char* */
    FIELD_TYPE_ADD_IMMUNITY = 6,
    /* const char* */
    FIELD_ITEM_ADD_FORM = 7,
    /* const char* */
    FIELD_ITEM_DEFAULT_FORM = 8,
    /* const char* */
    FIELD_ITEM_ADD_TRAIT = 9,
    /* item_CanHold,void* */
    FIELD_ITEM_CAN_HOLD_FN = 10,
    /* item_CanUseOn,void* */
    FIELD_ITEM_CAN_USE_ON_FN = 11,
    /* const char* */
    FIELD_MOVE_TYPE = 12,
    /* MoveCategory */
    FIELD_MOVE_CATEGORY = 13,
    /* uint32_t */
    FIELD_MOVE_POWER = 14,
    /* double */
    FIELD_MOVE_ACCURACY = 15,
    /* const char* */
    FIELD_MOVE_ADD_TRAIT = 16,
    /* move_CanUse,void* */
    FIELD_MOVE_CAN_USE_FN = 17,
    /* move_GetType,void* */
    FIELD_MOVE_GET_TYPE_FN = 18,
    /* move_GetCategory,void* */
    FIELD_MOVE_GET_CATEGORY_FN = 19,
    /* uint8_t */
    FIELD_MOVE_PP_BASE = 20,
    /* const char* */
    FIELD_FORM_TYPE_1 = 21,
    /* const char* */
    FIELD_FORM_TYPE_2 = 22,
    /* const char* */
    FIELD_FORM_ABILITY_1 = 23,
    /* const char* */
    FIELD_FORM_ABILITY_2 = 24,
    /* const char* */
    FIELD_FORM_HIDDEN_ABILITY = 25,
    /* Stat,uint16_t */
    FIELD_FORM_STAT = 26,
    /* Form* */
    FIELD_SPECIES_ADD_FORM = 27,
    /* const char* */
    FIELD_SPECIES_DEFAULT_FORM = 28,
    /* uint32_t */
    FIELD_SPECIES_REGIONAL_NUMBER = 29,
    /* uint32_t */
    FIELD_SPECIES_NATIONAL_HINT = 30,
    /* uint16_t,const char* */
    FIELD_SPECIES_ADD_LEVEL_MOVE = 31,
    /* const char* */
    FIELD_SPECIES_ADD_MACHINE_MOVE = 32,
    /* const char* */
    FIELD_ABILITY_ADD_LIFETIME_TIED = 40
};

POKEMONSMS_COMMON_API void Type_setField(Type*,Field,...);
POKEMONSMS_COMMON_API void Move_setField(Move*,Field,...);
POKEMONSMS_COMMON_API void Ability_setField(Ability*,Field,...);
POKEMONSMS_COMMON_API void Form_setField(Form*,Field,...);
POKEMONSMS_COMMON_API void Species_setField(PokemonSpecies*,Field,...);
POKEMONSMS_COMMON_API void Item_setField(Item*,Field,...);
POKEMONSMS_COMMON_API void Type_register(Type*);
POKEMONSMS_COMMON_API void Move_register(Move*);
POKEMONSMS_COMMON_API void Ability_register(Ability*);
POKEMONSMS_COMMON_API void Species_register(PokemonSpecies*);
POKEMONSMS_COMMON_API void Item_register(Item*);



#ifdef __cplusplus
}
#endif

#endif
