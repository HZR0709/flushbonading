#include <string.h>
#include "filter_registry.h"
#include "filter.h" 

// 对外无须暴露
typedef struct {
    const char *name;
    filter_func_t func;
} FilterEntry;


// 所有可用过滤器在这里注册
static FilterEntry g_filters[] = {
    { "size", filter_by_size },
    { "type", filter_by_type },
    { "name", filter_by_name },
};

filter_func_t filter_registry_find(const char* name) {
    size_t n = sizeof(g_filters) / sizeof(g_filters[0]);

    for (size_t i = 0; i < n; i++) {
        if (strcmp(g_filters[i].name, name) == 0) {
            return g_filters[i].func;
        }
    }
    return NULL;
}