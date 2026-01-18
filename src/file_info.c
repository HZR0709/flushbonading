#include "file_info.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>     // basename
#include <unistd.h>

/* 内部函数：根据 mode 判断文件类型 */
static FileType get_file_type(mode_t mode)
{
    if (S_ISREG(mode)) return FILE_TYPE_REG;
    if (S_ISDIR(mode)) return FILE_TYPE_DIR;
    if (S_ISLNK(mode)) return FILE_TYPE_LNK;
    return FILE_TYPE_OTHER;
}

FileInfo *file_info_create(const char *path)
{
    struct stat st;

    if (!path) {
        return NULL;
    }

    /* 使用 lstat：不跟随符号链接 */
    if (lstat(path, &st) != 0) { 
        perror("lstat");
        return NULL;
    }

    FileInfo *info = malloc(sizeof(FileInfo));
    if (!info) {
        return NULL;
    }

    memset(info, 0, sizeof(FileInfo));

    /* 保存完整路径（深拷贝） */
    info->path = strdup(path);
    if (!info->path) {
        free(info);
        return NULL;
    }

    /* basename 可能修改参数，必须用临时副本 */
    char *path_copy = strdup(path);
    if (!path_copy) {
        free(info->path);
        free(info);
        return NULL;
    }

    info->name = strdup(basename(path_copy));
    free(path_copy);

    if (!info->name) {
        free(info->path);
        free(info);
        return NULL;
    }

    info->size  = st.st_size;
    info->mode  = st.st_mode;
    info->mtime = st.st_mtime;
    info->type  = get_file_type(st.st_mode);

    return info;
}

void file_info_destroy(FileInfo *info)
{
    if (!info) return;

    free(info->name);
    free(info->path);
    free(info);
}
