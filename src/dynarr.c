#include "../include/dynarr.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define DYNARR_INIT_CAP 4

void dynarr_init(DynArr *v, size_t elem_size, DynArrDropFunc drop)
{
    v->len = 0;
    v->cap = DYNARR_INIT_CAP;
    v->elem_size = elem_size;
    v->drop = drop;
    v->data = malloc(v->cap * v->elem_size);
}

void dynarr_push(DynArr *v, const void *elem)
{
    if (v->len == v->cap) {
        v->cap *= 2;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }

    void *dst = (char *)v->data + v->len * v->elem_size;
    memcpy(dst, elem, v->elem_size);
    v->len++;
}

void dynarr_pop(DynArr *v)
{
    if (v->len == 0)
        return;

    if (v->drop) {
        void *elem = (char *)v->data + (v->len - 1) * v->elem_size;
        v->drop(elem);
    }
    v->len--;

    if (v->len < v->cap / 4 && v->cap > DYNARR_INIT_CAP) {
        v->cap /= 2;
        if (v->cap < DYNARR_INIT_CAP)
            v->cap = DYNARR_INIT_CAP;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }
}

void *dynarr_get(DynArr *v, size_t index)
{
    if (index >= v->len)
        return NULL;

    return (char *)v->data + index * v->elem_size;
}

void dynarr_free(DynArr *v)
{
    if (v->drop) {
        for (size_t i = 0; i < v->len; i++) {
            void *elem = (char *)v->data + i * v->elem_size;
            v->drop(elem);
        }
    }

    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    v->elem_size = 0;
}

void dynarr_clear(DynArr *v)
{
    if (v->drop) {
        for (size_t i = 0; i < v->len; i++) {
            void *elem = (char *)v->data + i * v->elem_size;
            v->drop(elem);
        }
    }

    v->len = 0;
    if (v->cap > DYNARR_INIT_CAP) {
        v->cap = DYNARR_INIT_CAP;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }
}

size_t dynarr_size(DynArr *v)
{
    return v->len;
}

void *dynarr_back(DynArr *v)
{
    if (v->len == 0)
        return NULL;

    return (char *)v->data + (v->len - 1) * v->elem_size;
}

bool dynarr_empty(DynArr *v)
{
    return v->len == 0;
}

void dynarr_insert(DynArr *v, size_t index, const void *elem)
{
    if (index > v->len)
        return;

    if (v->len == v->cap) {
        v->cap *= 2;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }

    void *src = (char *)v->data + index * v->elem_size;
    void *dst = (char *)v->data + (index + 1) * v->elem_size;
    memmove(dst, src, (v->len - index) * v->elem_size);

    dst = (char *)v->data + index * v->elem_size;
    memcpy(dst, elem, v->elem_size);
    v->len++;
}

void dynarr_remove(DynArr *v, size_t index)
{
    if (index >= v->len)
        return;

    if (v->drop) {
        void *elem = (char *)v->data + index * v->elem_size;
        v->drop(elem);
    }

    void *src = (char *)v->data + (index + 1) * v->elem_size;
    void *dst = (char *)v->data + index * v->elem_size;
    memmove(dst, src, (v->len - index - 1) * v->elem_size);

    v->len--;
    if (v->len < v->cap / 4 && v->cap > DYNARR_INIT_CAP) {
        v->cap /= 2;
        if (v->cap < DYNARR_INIT_CAP)
            v->cap = DYNARR_INIT_CAP;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }
}

void dynarr_resize(DynArr *v, size_t new_size)
{
    if (new_size == v->len)
        return;

    if (new_size > v->cap) {
        v->cap = new_size;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }

    if (new_size < v->len && v->drop) {
        for (size_t i = new_size; i < v->len; i++) {
            void *elem = (char *)v->data + i * v->elem_size;
            v->drop(elem);
        }
    }
    v->len = new_size;
}

void dynarr_reserve(DynArr *v, size_t new_cap)
{
    if (new_cap <= v->cap)
        return;

    v->cap = new_cap;
    v->data = realloc(v->data, v->cap * v->elem_size);
}

void dynarr_shrink_to_fit(DynArr *v)
{
    if (v->len == 0)
        v->cap = DYNARR_INIT_CAP;
    else
        v->cap = v->len;
    v->data = realloc(v->data, v->cap * v->elem_size);
}

void dynarr_copy(DynArr *dest, const DynArr *src)
{
    if (!dest || !src || dest == src)
        return;

    dest->elem_size = src->elem_size;
    dest->len = src->len;
    dest->cap = src->len;
    dest->drop = src->drop;

    dest->data = malloc(dest->cap * dest->elem_size);
    memcpy(dest->data, src->data, src->len * src->elem_size);
}

void dynarr_move(DynArr *dest, DynArr *src)
{
    *dest = *src;

    src->data = NULL;
    src->len = 0;
    src->cap = 0;
    src->elem_size = 0;
    src->drop = NULL;
}

void dynarr_swap(DynArr *v1, DynArr *v2)
{
    DynArr temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

void dynarr_extend(DynArr *dest, const DynArr *src)
{
    if ((dest == src) || (dest->elem_size != src->elem_size))
        return;

    if (dest->cap < dest->len + src->len) {
        dest->cap = dest->len + src->len;
        dest->data = realloc(dest->data, dest->cap * dest->elem_size);
    }

    memcpy((char *)dest->data + dest->len * dest->elem_size, src->data, src->len * src->elem_size);
    dest->len += src->len;
}

ssize_t dynarr_find(const DynArr *v, const void *elem)
{
    if (!v || !elem)
        return -1;

    for (size_t i = 0; i < v->len; ++i) {
        if (memcmp((char *)v->data + i * v->elem_size,
                   elem,
                   v->elem_size) == 0)
            return (ssize_t)i;
    }

    return -1;
}

void dynarr_sort(DynArr *v, int (*cmp)(const void *, const void *))
{
    if (!v || !cmp)
        return;

    qsort(v->data, v->len, v->elem_size, cmp);
}
