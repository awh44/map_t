#include <stdio.h>

#include "dict_t.h"

int main(void)
{
	dict_t *dict = dict_initialize(256);

	dict_put(dict, "hello", 10);
	dict_put(dict, "whyyyy", 37);
	dict_put(dict, "whyyyy", 99);

	uint64_t val1, val2;
	dict_get(dict, "hello", &val1);
	dict_get(dict, "whyyyy", &val2);

	printf("hello: %lu\nwhyyyy: %lu\n", val1, val2);

	dict_uninitialize(dict);
	return 0;
}
