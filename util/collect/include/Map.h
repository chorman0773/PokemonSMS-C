/*
 * Map.h
 *
 *  Created on: Sep. 20, 2019
 *      Author: connor
 */

#ifndef INCLUDE_INTERNAL_MAP_H_
#define INCLUDE_INTERNAL_MAP_H_

#ifdef __cplusplus
extern"C"{
#endif

#include <tigame/ComTypes.h>
#include <stddef.h>

typedef struct TreeMap TreeMap;

void free_dtor(void* alloc_data,void* value);

void* map_get(TreeMap* map,const void* key);
void map_put(TreeMap* map,const void* key,void* value);
TreeMap* map_new(void* data,tigame_bool(*cmpFn)(const void*,const void*),void(*value_dtor)(void* alloc_data,void* value),void(*key_dtor)(void* alloc_data,void* key));
void map_free(TreeMap* map);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_INTERNAL_MAP_H_ */
