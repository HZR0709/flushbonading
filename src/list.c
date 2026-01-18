#include "list.h"
#include <stdlib.h>

List *list_create(void)
{
    List *list = malloc(sizeof(List));
    if (!list) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

int list_push_back(List *list, void *data)
{
    if (!list) return -1;

    ListNode *node = malloc(sizeof(ListNode));
    if (!node) {
        return -1;
    }

    node->data = data;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return 0;
}

void list_foreach(List *list, ListForEachFn fn, void *arg)
{
    if (!list || !fn) return;

    ListNode *cur = list->head;
    while (cur) {
        fn(cur->data, arg);
        cur = cur->next;
    }
}

void list_destroy(List *list, void (*data_free)(void *))
{
    if (!list) return;

    ListNode *cur = list->head;
    while (cur) {
        ListNode *next = cur->next;

        if (data_free) {
            data_free(cur->data);
        }

        free(cur);
        cur = next;
    }

    free(list);
}
