#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/* 链表节点 */
typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;

/* 链表本体 */
typedef struct {
    ListNode *head;
    ListNode *tail;
    size_t    size;
} List;

/* 遍历回调 */
typedef void (*ListForEachFn)(void *data, void *arg);

/* 创建链表 */
List *list_create(void);

/* 尾插元素 */
int list_push_back(List *list, void *data);

/* 遍历链表 */
void list_foreach(List *list, ListForEachFn fn, void *arg);

/* 销毁链表（data_free 可为 NULL） */
void list_destroy(List *list, void (*data_free)(void *));
#endif
