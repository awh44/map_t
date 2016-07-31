#ifndef _CHARPTR_UINT64_MAP_H_
#define _CHARPTR_UINT64_MAP_H_

#include "map.h"

#include <stdint.h>

typedef char *charptr;
MAKE_MAP_H(map, charptr, uint64_t)

#endif
