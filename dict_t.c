#include <stdio.h>


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dict_t.h"

typedef struct dict_node_t
{
	char *key;
	uint64_t value;
	struct dict_node_t *next;
} dict_node_t;

struct dict_t
{
	size_t num_buckets;
	dict_node_t **buckets;
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

dict_t *dict_initialize(size_t buckets)
{
	dict_t *dict = malloc(sizeof *dict);
	if (dict == NULL)
	{
		return NULL;
	}

	dict->num_buckets = buckets;
	dict->buckets = calloc(dict->num_buckets, sizeof *dict->buckets);
	if (dict->buckets == NULL)
	{
		free(dict);
		return NULL;
	}

	return dict;
}

void dict_uninitialize(dict_t *dict)
{
	size_t i;
	dict_clear(dict);
	free(dict->buckets);
	free(dict);
}

dict_status_t dict_put(dict_t *dict, char *key, uint64_t value)
{
	size_t bucket = hash(key) % dict->num_buckets;
	dict_node_t *node = dict->buckets[bucket];
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
		node->next = dict->buckets[bucket];
		dict->buckets[bucket] = node;
	}

	node->value = value_copy(value);
	
	return DICT_SUCCESS;
}

dict_status_t dict_get(dict_t *dict, char *key, uint64_t *value)
{
	size_t bucket = hash(key) % dict->num_buckets;
	dict_node_t *node = dict->buckets[bucket];

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

void dict_clear(dict_t *dict)
{
	size_t i;
	for (i = 0; i < dict->num_buckets; i++)
	{
		dict_node_t *node = dict->buckets[i];
		while (node != NULL)
		{
			dict_node_t *tmp_node = node->next;
			
			key_uninitialize(node->key);
			value_uninitialize(node->value);
			free(node);
		
			node = tmp_node;
		}
		dict->buckets[i] = NULL;
	}
}
