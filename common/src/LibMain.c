#include <CommonLibrary.h>
#include <lua.h>
#include <Pokemon.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include <stdatomic.h>

struct Dispatch{
    void(*callback)(void*,uint64_t,lua_State*);
    void* handle;
    void(*handle_dtor)(void*);
    uint64_t key;
    pthread_cond_t exit_condition;
};

static void(*g_register_fns)(lua_State*,void*);
static void* g_handle;

static lua_State* common_lua_thread;
static pthread_t common_dispatcher_thread;
static LinkedList* dispatcher_queue;
static pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t queue_condition = PTHREAD_COND_INITIALIZER;
static atomic_flag exit_flag = ATOMIC_FLAG_INIT;

static void *l_alloc (void *ud, void *ptr, size_t osize,size_t nsize) {
    if (nsize == 0) {
        free(ptr);
        return NULL;
    }
    else
        return realloc(ptr, nsize);
}

static void common_run(void){
    pthread_mutex_lock(&queue_lock);
    dispatcher_queue = LinkedList_new(NULL);
    g_register_fns(common_lua_thread,g_handle);
    pthread_mutex_unlock(&queue_lock);
    while(!atomic_flag_test_and_set(&exit_flag)){
        pthread_mutex_lock(&queue_lock);
        do{
            Iterator* it = LinkedList_begin(dispatcher_queue);
            if(!it)
                pthread_cond_wait(&queue_condition,&queue_lock);
        }while(!it);
        struct Dispatch* dispatch = (struct Dispatch*)LinkedList_dereference(it);
        LinkedList_remove(it);
        pthread_mutex_unlock(&queue_lock);
        dispatch->callback(dispatch->handle,dispatch->key,common_lua_thread);
    }
}


void PokemonSMS_Common_OnInit(lua_State* state){
    puts("pokemonsms-common: OnInit");
    g_register_fns = register_fns;
    g_handle = handle;
    lua_newthread = lua_newthread(state);
}

void PokemonSMS_Common_Load(){
    puts("pokemonsms-common: Load");
}

pthread_cond_t* PokemonSMS_Common_Dispatch(void(*callback)(void*,uint64_t,lua_State*),void* handle,void(*handle_dtor)(void*)){
    pthread_mutex_lock(&queue_lock);
    
}
