#include <string.h>

#include "map.h"
#include "charptr_uint64_map.h"

static void charptr_init(charptr *k)
{
}

static void charptr_uninit(charptr *k)
{
	free(*k);
}

static void charptr_copy(charptr *k1, charptr *k2)
{
	*k1 = strdup(*k2);
}

static unsigned short charptr_equal(charptr *k1, charptr *k2)
{
	return strcmp(*k1, *k2) == 0;
}

/**
  * Performs a hash of a string based on the djb2 algorithm - see http://www.cse.yorku.ca/~oz/hash.html
  * @param str the string to be hashed
  * @return the hash value of the string
  */
static size_t charptr_hash(charptr *key)
{
    size_t hash_val = 5381;
    char c;
	charptr local = *key;

    while ((c = *local++))
    {
        hash_val = ((hash_val << 5) + hash_val) + c;
    }

    return hash_val;
}

static void uint64_init(uint64_t *value)
{
}

static void uint64_uninit(uint64_t *value)
{
}

static void uint64_copy(uint64_t *v1, uint64_t *v2)
{
	*v1 = *v2;
}

MAKE_MAP_C(map, charptr, uint64_t,
           charptr_init, charptr_uninit, charptr_copy, charptr_equal, charptr_hash,
		   uint64_init, uint64_uninit, uint64_copy)
