#include <stdio.h>

//#include "map.h"
#include "charptr_uint64_map.h"

int main(void)
{
	map_t *map = map_initialize(256);

	map_put(map, "hello", 10);
	map_put(map, "whyyyy", 37);
	map_put(map, "whyyyy", 99);

	uint64_t val1, val2;
	map_get(map, "hello", &val1);
	map_get(map, "whyyyy", &val2);

	printf("hello: %lu\nwhyyyy: %lu\n", val1, val2);

	map_uninitialize(map);
	return 0;
}
