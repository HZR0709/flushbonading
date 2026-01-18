#ifndef __FILTER_REGISTRY_H
#define __FILTER_REGISTRY_H

#include "filter.h"

filter_func_t filter_registry_find(const char* name);

#endif