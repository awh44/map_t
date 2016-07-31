#include <stdio.h>


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

typedef struct map_node_t
{
	char *key;
	uint64_t value;
	struct map_node_t *next;
} map_node_t;

struct map_t
{
	size_t num_buckets;
	map_node_t **buckets;
};

static void key_uninitialize(char *key)
{
	free(key);
}

static uint8_t key_equal(char *key1, char *key2)
{
	return strcmp(key1, key2) == 0;
}

static char *key_copy(char *key)
{
	return strdup(key);
}

static void value_uninitialize(uint64_t value)
{
}

static uint64_t value_copy(uint64_t value)
{
	return value;
}

/**
  * Performs a hash of a string based on the djb2 algorithm - see http://www.cse.yorku.ca/~oz/hash.html
  * @param str the string to be hashed
  * @return the hash value of the string
  */
static size_t hash(char *key)
{
    size_t hash_val = 5381;
    char c;

    while ((c = *key++))
    {
        hash_val = ((hash_val << 5) + hash_val) + c;
    }

    return hash_val;
}

map_t *map_initialize(size_t buckets)
{
	map_t *map = malloc(sizeof *map);
	if (map == NULL)
	{
		return NULL;
	}

	map->num_buckets = buckets;
	map->buckets = calloc(map->num_buckets, sizeof *map->buckets);
	if (map->buckets == NULL)
	{
		free(map);
		return NULL;
	}

	return map;
}

void map_uninitialize(map_t *map)
{
	map_clear(map);
	free(map->buckets);
	free(map);
}

map_status_t map_put(map_t *map, char *key, uint64_t value)
{
	size_t bucket = hash(key) % map->num_buckets;
	map_node_t *node = map->buckets[bucket];
	while (node != NULL && !key_equal(key, node->key))
	{
		node = node->next;
	}

	//No existing node with that key
	if (node == NULL)
	{
		node = malloc(sizeof *node);
		if (node == NULL)
		{
			return DICT_OUT_OF_MEMORY;
		}
		
		//can fail if this is a dynamic allocation...hmmm
		node->key = key_copy(key);
		node->next = map->buckets[bucket];
		map->buckets[bucket] = node;
	}

	node->value = value_copy(value);
	
	return DICT_SUCCESS;
}

map_status_t map_get(map_t *map, char *key, uint64_t *value)
{
	size_t bucket = hash(key) % map->num_buckets;
	map_node_t *node = map->buckets[bucket];

	while (node != NULL && !key_equal(key, node->key))
	{
		node = node->next;
	}

	if (node == NULL)
	{
		return DICT_NOT_FOUND;
	}

	*value = node->value;

	return DICT_SUCCESS;
}

void map_clear(map_t *map)
{
	size_t i;
	for (i = 0; i < map->num_buckets; i++)
	{
		map_node_t *node = map->buckets[i];
		while (node != NULL)
		{
			map_node_t *tmp_node = node->next;
			
			key_uninitialize(node->key);
			value_uninitialize(node->value);
			free(node);
		
			node = tmp_node;
		}
		map->buckets[i] = NULL;
	}
}
