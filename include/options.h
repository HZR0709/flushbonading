#ifndef OPTIONS_H
#define OPTIONS_H

#include <sys/types.h>
#include "file_info.h"

typedef struct {
    const char *path;
    off_t min_size;
    FileType type;
    const char *name_substr;
} ScanOptions;

int parse_args(int argc, char **argv, ScanOptions *opts);

#endif