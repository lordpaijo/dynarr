#ifndef DYNARR_H
#define DYNARR_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

typedef void (*DynArrDropFunc)(void *);

typedef struct {
    void   *data;
    size_t  len;
    size_t  cap;
    size_t  elem_size;
    DynArrDropFunc drop;
} DynArr; // Dynamic Array (I'm dumb at naming)

void  dynarr_init(DynArr *v, size_t elem_size, DynArrDropFunc drop);
void  dynarr_push(DynArr *v, const void *elem);
void  dynarr_pop(DynArr *v);
void *dynarr_get(DynArr *v, size_t index);
void  dynarr_free(DynArr *v);
void  dynarr_clear(DynArr *v);
size_t dynarr_size(DynArr *v);
void *dynarr_back(DynArr *v);
bool dynarr_empty(DynArr *v);
void dynarr_insert(DynArr *v, size_t index, const void *elem);
void dynarr_remove(DynArr *v, size_t index);
void dynarr_resize(DynArr *v, size_t new_size);
void dynarr_reserve(DynArr *v, size_t new_cap);
void dynarr_shrink_to_fit(DynArr *v);
void dynarr_copy(DynArr *dest, const DynArr *src);
void dynarr_move(DynArr *dest, DynArr *src);
void dynarr_swap(DynArr *v1, DynArr *v2);
void dynarr_extend(DynArr *dest, const DynArr *src);
ssize_t dynarr_find(const DynArr *v, const void *elem);
void dynarr_sort(DynArr *v, int (*cmp)(const void *, const void *));

#endif
