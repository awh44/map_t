#ifndef _DICTIONARY_T_H_
#define _DICTIONARY_T_H_

#include <stdint.h>

typedef struct map_t map_t;

typedef enum
{
	DICT_SUCCESS = 0,
	DICT_OUT_OF_MEMORY,
	DICT_NOT_FOUND
} map_status_t;


map_t *map_initialize();
void map_uninitialize(map_t *map);
map_status_t map_put(map_t *map, char *key, uint64_t value);
map_status_t map_get(map_t *map, char *key, uint64_t *value);
void map_clear(map_t *map);

#endif
