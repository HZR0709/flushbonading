#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "file_info.h"
#include "list.h"
#include "filter.h"

/* ========== 输出 ========== */

static const char *type_to_str(FileType t)
{
    switch (t) {
    case FILE_TYPE_REG: return "REG";
    case FILE_TYPE_DIR: return "DIR";
    case FILE_TYPE_LNK: return "LNK";
    default:            return "OTH";
    }
}

static void print_file(void *data, void *arg)
{
    (void)arg;
    FileInfo *info = (FileInfo *)data;
    printf("%-3s %10ld %s\n",
           type_to_str(info->type),
           info->size,
           info->path);
}

/* ========== 帮助 ========== */

static void print_usage(const char *prog)
{
    printf("Usage: %s <path> [options]\n", prog);
    printf("Options:\n");
    printf("  --type f|d|l       file type\n");
    printf("  --min-size N       minimum size (bytes)\n");
    printf("  --max-size N       maximum size (bytes)\n");
}

void file_list_print(const List *list)
{
    if (!list)
        return;

    const ListNode *node = list->head;
    while (node) {
        const FileInfo *info = node->data;
        if (info) {
            printf("%s (%ld bytes)\n",
                   info->path,
                   (long)info->size);
        }
        node = node->next;
    }
}

/* ========== main ========== */

int main(int argc, char *argv[])
{
    ScanOptions opts;
    if (parse_args(argc, argv, &opts) != 0) {
        fprintf(stderr, "Usage: %s path [--min-size=N] [--type=regular|dir] [--name=xxx]\n", argv[0]);
        return 1;
    }

    FilterSet *filters = filter_set_create();
    List *result = list_create();

    build_filters_from_opts(&opts, filters);
    scan_directory(opts.path, filters, result);

    file_list_print(result);

    filter_set_destroy(filters);
    list_destroy(result, NULL);

    return 0;
}
