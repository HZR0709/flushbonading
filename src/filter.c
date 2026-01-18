#include "filter.h"
#include "filter_registry.h"
#include <stdlib.h>

/* free 回调：释放 FilterNode 和其 arg（如果是 malloc 的） */
static void free_filter(void *data)
{
    if (!data) return;
    FilterNode *f = (FilterNode *)data;
    if (f->arg) {
        free(f->arg);
    }
    free(f);
}

FilterSet *filter_set_create(void)
{
    FilterSet *set = malloc(sizeof(FilterSet));
    if (!set) return NULL;

    set->filters = list_create();
    if (!set->filters) {
        free(set);
        return NULL;
    }

    return set;
}

int filter_set_add(FilterSet *set, filter_func_t fn, void *arg)
{
    if (!set || !fn) return -1;

    FilterNode *node = malloc(sizeof(FilterNode));
    if (!node) return -1;

    node->fn = fn;
    node->arg = arg;
    node->next = NULL; /* unused since stored in List */

    return list_push_back(set->filters, node);
}

bool filter_set_apply(const FilterSet *set, const FileInfo *info)
{
    if (!set || !info) return true;

    ListNode *cur = set->filters->head;
    while (cur) {
        FilterNode *filter = (FilterNode *)cur->data;
        if (!filter) { cur = cur->next; continue; }
        if (filter->fn && !filter->fn(info, filter->arg)) {
            return false;
        }
        cur = cur->next;
    }

    return true;
}

void filter_set_destroy(FilterSet *set)
{
    if (!set) return;

    list_destroy(set->filters, free_filter);
    free(set);
}

/* build_filters_from_opts：注意为每个 numeric arg malloc，并为 name 做 strdup，这样 free_filter 可以统一 free */
void build_filters_from_opts(const ScanOptions *opts, FilterSet *set) {
    if (!opts || !set) return;

    if (opts->min_size > 0) {
        filter_func_t fn = filter_registry_find("size");
        if (fn) {
            off_t *arg = malloc(sizeof(off_t));
            if (arg) {
                *arg = opts->min_size;
                filter_set_add(set, fn, arg);
            }
        }
    }

    if (opts->type != FILE_TYPE_OTHER) {
        filter_func_t fn = filter_registry_find("type");
        if (fn) {
            FileType *arg = malloc(sizeof(FileType));
            if (arg) {
                *arg = opts->type;
                filter_set_add(set, fn, arg);
            }
        }  
    }

    if (opts->name_substr) {
        filter_func_t fn = filter_registry_find("name");
        if (fn) {
            char *arg = strdup(opts->name_substr); /* strdup so we own memory */
            if (arg) {
                filter_set_add(set, fn, arg);
            }
        } 
    }
}

// 按文件大小过滤
bool filter_by_size(const FileInfo *info, const void *arg)
{
    if (!info || !arg)
        return true;   // 不设条件则默认通过

    off_t min_size = *(const off_t *)arg;
    return info->size >= min_size;
}


// 按文件类型过滤
bool filter_by_type(const FileInfo *info, const void *arg)
{
    if (!info || !arg)
        return true;

    FileType type = *(const FileType *)arg;
    return info->type == type;
}

// 按文件名子串过滤
bool filter_by_name(const FileInfo *info, const void *arg)
{
    if (!info || !arg)
        return true;

    const char *substr = (const char *)arg;
    return strstr(info->name, substr) != NULL;
}
