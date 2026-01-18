#include <string.h>
#include <stdlib.h>
#include "options.h"

int parse_args(int argc, char **argv, ScanOptions *opts) {
    memset(opts, 0, sizeof(*opts));
    opts->type = FILE_TYPE_OTHER;

    if (argc < 2) return -1;

    opts->path = argv[1];

    for (int i = 2; i < argc; i++) {
        if (strncmp(argv[i], "--min-size=", 11) == 0) {
            opts->min_size = atoll(argv[i] + 11);
        } else if (strncmp(argv[i], "--type=", 7) == 0) {
            if (strcmp(argv[i] + 7, "regular") == 0) {
                opts->type = FILE_TYPE_REG;
            } else if (strcmp(argv[i] + 7, "dir") == 0) {
                opts->type = FILE_TYPE_DIR;
            }
        } else if (strncmp(argv[i], "--name=", 7) == 0) {
            opts->name_substr = argv[i] + 7;
        }
    }
    return 0;
}