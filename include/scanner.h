#ifndef SCANNER_H
#define SCANNER_H

#include "list.h"
#include "filter.h"

/**
 * @brief  递归扫描目录
 * @param  path    起始路径
 * @param  filters 过滤器集合（可为 NULL）
 * @param  result  输出结果 List<FileInfo*>
 * @return 0 成功，-1 失败
 */
int scan_directory(const char *path,
                   const FilterSet *filters,
                   List *result);

#endif
