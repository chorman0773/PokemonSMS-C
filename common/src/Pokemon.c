
#include <Pokemon.h>
#include <json.h>
#include <Map.h>

static int _trait_exists;

static int type_weekness;
static int type_resistance;
static int type_immunity;

struct Type{
    char* loc;
    struct json_object* name;
    int refcount;
    event_handler* bus;
    void* bus_handle;
    TreeMap* type_matchups;
};

struct Move{
    char* loc;
    struct json_object* name;
    struct json_object* description;
    int refcount;
    event_handler* bus;
    void* bus_handle;
    char* type;
    TreeMap* traits;
    move_CanUse* canUse;
    void* canUse_handle;
    move_GetType* getType;
    void* getType_handle;
    move_GetCategory* getCategory;
    void* getCategory_handle;
};

struct Ability{
    char* loc;
    struct json_object* name;
    struct json_object* description;
    int refcount;
    event_handler* bus;
    void* bus_handle;
    TreeMap* lifetime_tied;
};

struct Form{
    char* id;
    struct json_object* name;
    int refcount;
    event_handler* bus;
    void* bus_handle;
    uint16_t stats[6];
    char* types[2];
    char* abilities[3];
};

struct PokemonSpecies{
    char* id;
    struct json_object* name;
    struct json_object* description;
    int refcount;
    event_handler* bus;
    void* bus_handle;
    TreeMap* forms;
    char* defForm;
    int32_t regionalNumber;
    uint32_t natDexHint;
    pokemon_GetForm* getForm;
    void* getForm_handle;
};

struct Item{
    char* id;
    struct json_object* name;
    struct json_object* description;
    int refcount;
    event_handler* bus;
    void* bus_handle;
    TreeMap* forms;
    char* defForm;
    TreeMap* traits;
    item_CanHold* canHold;
    void* canHold_handle;
    item_CanUseOn* canUse;
    void* canUseOn_handle;
};




POKEMONSMS_COMMON_API Type* Type_new();
POKEMONSMS_COMMON_API Move* Move_new();
POKEMONSMS_COMMON_API Ability* Ability_new();
POKEMONSMS_COMMON_API Form* Form_new();
POKEMONSMS_COMMON_API PokemonSpecies* Species_new();
POKEMONSMS_COMMON_API Item* Item_new();

POKEMONSMS_COMMON_API Type* Type_dup(Type*);
POKEMONSMS_COMMON_API Move* Move_dup(Move*);
POKEMONSMS_COMMON_API Ability* Ability_dup(Ability*);
POKEMONSMS_COMMON_API Form* Form_dup(Form*);
POKEMONSMS_COMMON_API Item* Item_dup(Item*);

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

POKEMONSMS_COMMON_API void Type_free(Type*);
POKEMONSMS_COMMON_API void Move_free(Move*);
POKEMONSMS_COMMON_API void Ability_free(Ability*);
POKEMONSMS_COMMON_API void Species_free(PokemonSpecies*);
POKEMONSMS_COMMON_API void Form_free(Form*);
POKEMONSMS_COMMON_API void Item_free(Item*);
