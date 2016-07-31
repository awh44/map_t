#include <string.h>

#include "map.h"
#include "charptr_uint64_map.h"

static uint8_t charptr_equal(charptr k1, charptr k2)
{
	return strcmp(k1, k2) == 0;
}

/**
  * Performs a hash of a string based on the djb2 algorithm - see http://www.cse.yorku.ca/~oz/hash.html
  * @param str the string to be hashed
  * @return the hash value of the string
  */
static size_t charptr_hash(char *key)
{
    size_t hash_val = 5381;
    char c;

    while ((c = *key++))
    {
        hash_val = ((hash_val << 5) + hash_val) + c;
    }

    return hash_val;
}

static void uint64_uninit(uint64_t value)
{
}

static uint64_t uint64_copy(uint64_t value)
{
	return value;
}

MAKE_MAP_C(map, charptr, uint64_t, strdup, free, charptr_equal, charptr_hash, uint64_uninit, uint64_copy)
