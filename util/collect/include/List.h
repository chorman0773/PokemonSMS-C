#ifndef UTIL_COLLECT_LIST_H_2019_12_01_12_35
#define UTIL_COLLECT_LIST_H_2019_12_01_12_35

#include <stddef.h>

#ifdef __cplusplus
extern"C"{
#endif

typedef struct LinkedList LinkedList;
typedef struct Iterator Iterator;
typedef struct ArrayList ArrayList;

LinkedList* LinkedList_new(void(*deallocFn)(void*));
void LinkedList_free(LinkedList*);
Iterator* LinkedList_begin(LinkedList*);
Iterator* LinkedList_next(Iterator*);
Iterator* LinkedList_insert(Iterator*,void*);
void* LinkedList_dereference(Iterator*);
void LinkedList_remove(Iterator*);
Iterator* LinkedList_pushBack(LinkedList*,void*);
Iterator* LinkedList_back(LinkedList*);
Iterator* LinkedList_popBack(LinkedList*);

ArrayList* ArrayList_new(void(*destructor)(void*),void(*copy)(void*,const void*),void(*relocate)(void*,void*),size_t valLen);
void ArrayList_add(ArrayList*,const void*);
void* ArrayList_data(ArrayList*);
void* ArrayList_get(ArrayList*,size_t pos);
void ArrayList_free(ArrayList*);
size_t ArrayList_length(ArrayList*);
void ArrayList_clear(ArrayList*);

#ifdef __cplusplus
};
#endif

#endif
