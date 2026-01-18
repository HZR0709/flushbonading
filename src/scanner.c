#include "scanner.h"
#include "file_info.h"

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* 内部递归函数 */
static int scan_recursive(const char *path,
                          const FilterSet *filters,
                          List *result)
{
    DIR *dir = opendir(path);
    if (!dir) {
        return -1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {

        /* 跳过 . 和 .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath),
                 "%s/%s", path, entry->d_name);

        FileInfo *info = file_info_create(fullpath);
        if (!info) {
            continue;
        }

        /* 先过滤 */
        if (!filters || filter_set_apply(filters, info)) {
            list_push_back(result, info);
        } else {
            file_info_destroy(info);
            continue;
        }

        /* 如果是目录，递归 */
        if (info->type == FILE_TYPE_DIR) {
            scan_recursive(fullpath, filters, result);
        }
    }

    closedir(dir);
    return 0;
}

int scan_directory(const char *path,
                   const FilterSet *filters,
                   List *result)
{
    if (!path || !result) return -1;
    return scan_recursive(path, filters, result);
}
