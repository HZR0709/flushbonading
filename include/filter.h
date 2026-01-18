#ifndef FILTER_H
#define FILTER_H

#include <stdbool.h>
#include "file_info.h"
#include "list.h"
#include "options.h"

/* 单个过滤规则函数 */
typedef bool (*filter_func_t)(const FileInfo *info, const void *arg);

/* 一个过滤器节点 */
typedef struct filter_node {
    filter_func_t fn;
    void *arg;
    struct filter_node *next;
} FilterNode;

/* 过滤器集合 */
typedef struct {
    List *filters;   // List<FileFilter*>
} FilterSet;

/* 创建过  滤器集合 */
FilterSet *filter_set_create(void);

/* 添加一个过滤规则 */
int filter_set_add(FilterSet *set, filter_func_t fn, void *arg);

/* 应用所有过滤规则（AND 关系） */
bool filter_set_apply(const FilterSet *set, const FileInfo *info);

/* 销毁过滤器集合 */
void filter_set_destroy(FilterSet *set);

/* 具体过滤规则接口声明 */
bool filter_by_size(const FileInfo *info, const void *arg);
bool filter_by_type(const FileInfo *info, const void *arg);
bool filter_by_name(const FileInfo *info, const void *arg);

/* 构建过滤器集合（由 options 驱动） */
void build_filters_from_opts(const ScanOptions *opts, FilterSet *set);

#endif
