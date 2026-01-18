#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/* 文件类型 */
typedef enum {
    FILE_TYPE_REG,     // 普通文件
    FILE_TYPE_DIR,     // 目录
    FILE_TYPE_LNK,     // 符号链接
    FILE_TYPE_OTHER    // 其他
} FileType;

/* 文件信息结构体 */
typedef struct {
    char     *name;    // 文件名（不含路径）
    char     *path;    // 完整路径
    off_t     size;    // 文件大小
    mode_t    mode;    // 权限 + 类型
    time_t    mtime;   // 修改时间
    FileType  type;    // 文件类型
} FileInfo;

/* 从路径创建 FileInfo */
FileInfo *file_info_create(const char *path);

/* 释放 FileInfo */
void file_info_destroy(FileInfo *info);

#endif
