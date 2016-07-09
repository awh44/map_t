#ifndef _DICTIONARY_T_H_
#define _DICTIONARY_T_H_

#include <stdint.h>

struct dict_t;
typedef struct dict_t dict_t;

typedef enum
{
	DICT_SUCCESS = 0,
	DICT_OUT_OF_MEMORY,
	DICT_NOT_FOUND
} dict_status_t;


dict_t *dict_initialize();
void dict_uninitialize(dict_t *dict);
dict_status_t dict_put(dict_t *dict, char *key, uint64_t value);
dict_status_t dict_get(dict_t *dict, char *key, uint64_t *value);
void dict_clear(dict_t *dict);

#endif
