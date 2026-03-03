#ifndef H_TDS_LIST
#define H_TDS_LIST

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct lnode_t {
    struct lnode_t* next;
    struct lnode_t* prev;
    void* data;
} lnode_t;

typedef struct list_t {
    lnode_t* root;
    lnode_t* tail;
    size_t count;
} list_t;

static list_t* list_alloc() {
    list_t* list = malloc(sizeof(list_t));
    assert(list);
    list->root = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

static lnode_t* lnode_alloc() {
    lnode_t* node = malloc(sizeof(lnode_t));
    assert(node);
    node->next = NULL;
    node->prev = NULL;
    node->data = NULL;
    return node;
}

static void lnode_free(lnode_t* node) {
    if (!node) return;
    free(node);
}

static void list_free(list_t* list) {
   if (!list) return;
   lnode_t* iter = list->root;
   while (iter != NULL) {
       lnode_t* next = iter->next;
       lnode_free(iter);
       iter = next;
   }
   free(list);
}

static void list_push_front(list_t* list, void* value) {
    assert(list);
    lnode_t* nroot = lnode_alloc();
    nroot->data = value;
    nroot->next = list->root;
    nroot->prev = NULL;
    if (list->root) list->root->prev = nroot;
    else {
        list->tail = nroot;
    }
    list->root = nroot;
    list->count++;
}

static void list_push_back(list_t* list, void* value) {
    assert(list);
    lnode_t* ntail = lnode_alloc();
    ntail->data = value;
    ntail->next = NULL;
    ntail->prev = list->tail;
    if (list->tail) list->tail->next = ntail;
    else {
        list->root = ntail;
    }

    list->tail = ntail;
    list->count++;
}

static void* list_pop_front(list_t* list) {
    assert(list);
    assert(list->count);

    lnode_t* nroot = list->root->next;
    if (nroot) nroot->prev = NULL;
    else {
        list->tail = NULL;
    }

    void* result = list->root->data;
    lnode_free(list->root);
    list->root = nroot;

    list->count--;

    return result;
}

static void* list_pop_back(list_t* list) {
    assert(list);
    assert(list->count);

    lnode_t* ntail = list->tail->prev;
    if (ntail) ntail->next = NULL;
    else {
        list->root = NULL;
    }

    void* result = list->tail->data;
    lnode_free(list->tail);
    list->tail = ntail;

    list->count--;

    return result;
}

static lnode_t* list_at(list_t* list, size_t index) {
    assert(list);
    assert(index < list->count);

    if (index == 0) return list->root;

    if (index == list->count - 1) return list->tail;

    if (index < list->count / 2) {
        lnode_t* iter = list->root;
        for (size_t i = 0; i < index; ++i) {
            iter = iter->next;
        }
        return iter;
    }

    lnode_t* iter = list->tail;
    for (size_t i = list->count - 1; i > index; --i) {
        iter = iter->prev;
    }

    return iter;
}

static void* list_remove_at(list_t* list, size_t index) {
    assert(list);
    assert(index < list->count);

    if (index == 0) return list_pop_front(list);
    if (index == list->count - 1) return list_pop_back(list);

    lnode_t* node = list_at(list, index);

    void* result = node->data;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    lnode_free(node);
    list->count--;

    return result;
}

#endif // H_TDS_LIST
