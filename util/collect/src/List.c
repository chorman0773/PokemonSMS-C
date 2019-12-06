#include <List.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct Iterator{
    void* value;
    LinkedList* list;
    Iterator* next;
    Iterator* prev;
};
struct LinkedList{
    Iterator* begin;
    Iterator* back;
    void(*deallocFn)(void*);
};


LinkedList* LinkedList_new(void(*deallocFn)(void*)){
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->begin = NULL;
    list->deallocFn = deallocFn;
    return list;
}
void LinkedList_free(LinkedList* list){
    Iterator* next;
    for(Iterator* it = list->begin;it!=NULL;it=next){
        next = it->next;
        if(list->deallocFn)
            list->deallocFn(it->value);
        free(it);
    }
    free(list);
}
Iterator* LinkedList_begin(LinkedList* list){
    return list?list->begin:NULL;
}
Iterator* LinkedList_next(Iterator* it){
    return it->next;
}
Iterator* LinkedList_insert(Iterator* it,void* v){
    Iterator* nit = (Iterator*)malloc(sizeof(Iterator));
    nit->value = v;
    nit->list = it->list;
    nit->prev = it;
    nit->next = it->next;
    it->next = nit->next;
    if(!nit->next)
        it->list->back = nit;
    return nit;
}
void* LinkedList_dereference(Iterator* it){
    return it->value;
}

void LinkedList_remove(Iterator* it){
    if(it->next==NULL)
        it->list->back = it->prev;
    else
        it->next->prev = it->prev;
    if(it->prev==NULL)
        it->list->begin = it->next;
    else
        it->prev->next = it->next;
    if(it->list->deallocFn)
        it->list->deallocFn(it->value);
    free(it);
}
Iterator* LinkedList_pushBack(LinkedList* list,void* value){
    Iterator* it = (Iterator*) malloc(sizeof(Iterator));
    it->value = value;
    it->list = list;
    it->prev = list->back;
    it->next = NULL;
    if(list->back)
        list->back->next = it;
    else
        list->begin = it;
    list->back = it;
    return it;
}
Iterator* LinkedList_back(LinkedList* list){
    return list->back;
}
Iterator* LinkedList_popBack(LinkedList* list){
    LinkedList_remove(list->back);
    return list->back;
}


struct ArrayList{
    char* arr;
    size_t valSize;
    void(*valDtor)(void*);
    void(*valCopy)(void*,const void*);
    void(*valRelocate)(void*,void*);
    size_t length;
    size_t capacity;
};

ArrayList* ArrayList_new(void(*destructor)(void*),void(*copy)(void*,const void*),void(*relocate)(void*,void*),size_t valLen){
    ArrayList* arr = (ArrayList*) malloc(sizeof(ArrayList));
    arr->valDtor = destructor;
    arr->valCopy = copy;
    arr->valRelocate = relocate;
    arr->valSize = valLen;
    arr->length = 0;
    arr->capacity = 16;
    arr->arr = (char*) calloc(arr->capacity,valLen);
    return arr;
}
void ArrayList_add(ArrayList* arr,const void* val){
    if(arr->length==arr->capacity){
        arr->capacity *=2;
        char* narr = (char*) calloc(arr->capacity,arr->valSize);
        if(arr->valRelocate)
            for(size_t i = 0;i<arr->length;i++)
                arr->valRelocate(narr+(i*arr->valSize),arr->arr+(i*arr->valSize));
        else
            memcpy(narr,arr->arr,arr->length*arr->valSize);
        free(arr->arr);
        arr->arr = narr;
    }
    char* ptr = arr->arr + (arr->length*arr->valSize);
    if(arr->valCopy)
        arr->valCopy(ptr,val);
    else
        memcpy(ptr,val,arr->valSize);
    arr->length++;
}
void* ArrayList_data(ArrayList* arr){
    return arr->arr;
}
void* ArrayList_get(ArrayList* arr,size_t pos){
    return arr->arr+(pos*arr->valSize);
}
void ArrayList_free(ArrayList* arr){
    if(arr->valDtor)
        for(size_t i = 0;i<arr->length;i++)
            arr->valDtor(arr->arr+(i*arr->valSize));
    free(arr->arr);
    free(arr);
}

size_t ArrayList_length(ArrayList* arr){
    return arr->length;
}

void ArrayList_clear(ArrayList* arr){
    if(arr->valDtor)
        for(size_t i = 0;i<arr->length;i++)
            arr->valDtor(arr->arr+(i*arr->valSize));
    arr->length = 0;
}

