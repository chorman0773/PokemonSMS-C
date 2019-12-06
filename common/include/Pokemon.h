#ifdef COMMON_POKEMON_H_2019_12_05_18_05
#define COMMON_POKEMON_H_2019_12_05_18_05

#ifdef __cplusplus
extern"C"{
#endif
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

typedef uint32_t Field;

enum{
    FIELD_LOCATION = 0,
    FIELD_UNNAME = 1,
    FIELD_DESCRIPTION = 2,
    FIELD_BUS = 3,
    FIELD_TYPE_ADD_WEEKNESS = 4,
    FIELD_TYPE_ADD_RESISTANCE = 5,
    FIELD_TYPE_ADD_IMMUNITY = 6,
    FIELD_ITEM_ADD_FORM
};





#ifdef __cplusplus
}
#endif

#endif
