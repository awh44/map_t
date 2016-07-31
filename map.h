#ifndef _MAP_H_
#define _MAP_H_

#include <stdlib.h>

typedef enum
{
	MAP_SUCCESS = 0,
	MAP_OUT_OF_MEMORY,
	MAP_NOT_FOUND
} map_status_t;

#define MAKE_MAP_H(name, from, to)\
typedef struct name##_t name##_t;\
\
name##_t *name##_initialize();\
void name##_uninitialize(name##_t *map);\
map_status_t name##_put(name##_t *map, from key, to value);\
map_status_t name##_get(name##_t *map, from key, to *value);\
void name##_clear(name##_t *map);\

#define MAKE_MAP_C(name, from, to, key_copy, key_uninit, key_equal, key_hash, value_uninit, value_copy)\
typedef struct name##_node_t\
{\
	from key;\
	to value;\
	struct name##_node_t *next;\
} name##_node_t;\
\
struct name##_t\
{\
	size_t num_buckets;\
	name##_node_t **buckets;\
};\
\
name##_t *name##_initialize(size_t buckets)\
{\
	name##_t *map = malloc(sizeof *map);\
	if (map == NULL)\
	{\
		return NULL;\
	}\
\
	map->num_buckets = buckets;\
	map->buckets = calloc(map->num_buckets, sizeof *map->buckets);\
	if (map->buckets == NULL)\
	{\
		free(map);\
		return NULL;\
	}\
\
	return map;\
}\
\
void name##_uninitialize(name##_t *map)\
{\
	name##_clear(map);\
	free(map->buckets);\
	free(map);\
}\
\
map_status_t name##_put(name##_t *map, from key, to value)\
{\
	size_t bucket = key_hash(key) % map->num_buckets;\
	name##_node_t *node = map->buckets[bucket];\
	while (node != NULL && !key_equal(key, node->key))\
	{\
		node = node->next;\
	}\
\
	if (node == NULL)\
	{\
		node = malloc(sizeof *node);\
		if (node == NULL)\
		{\
			return MAP_OUT_OF_MEMORY;\
		}\
\
		node->key = key_copy(key);\
		node->next = map->buckets[bucket];\
		map->buckets[bucket] = node;\
	}\
\
	node->value = value_copy(value);\
\
	return MAP_SUCCESS;\
}\
\
map_status_t name##_get(name##_t *map, from key, to *value)\
{\
	size_t bucket = key_hash(key) % map->num_buckets;\
	name##_node_t *node = map->buckets[bucket];\
\
	while (node != NULL && !key_equal(key, node->key))\
	{\
		node = node->next;\
	}\
\
	if (node == NULL)\
	{\
		return MAP_NOT_FOUND;\
	}\
\
	*value = node->value;\
\
	return MAP_SUCCESS;\
}\
\
void name##_clear(name##_t *map)\
{\
	size_t i;\
	for (i = 0; i < map->num_buckets; i++)\
	{\
		name##_node_t *node = map->buckets[i];\
		while (node != NULL)\
		{\
			name##_node_t *tmp_node = node->next;\
\
			key_uninit(node->key);\
			value_uninit(node->value);\
			free(node);\
\
			node = tmp_node;\
		}\
		map->buckets[i] = NULL;\
	}\
}

#endif
