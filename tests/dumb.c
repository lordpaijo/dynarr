#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/dynarr.h"

/* ---------- helpers ---------- */

static void int_drop(void *elem)
{
    /* ints don't own memory */
    (void)elem;
}

static int int_cmp(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

/* ---------- tests ---------- */

int main(void)
{
    DynArr arr;

    /* init */
    dynarr_init(&arr, sizeof(int), int_drop);
    assert(dynarr_empty(&arr));
    assert(dynarr_size(&arr) == 0);

    /* push */
    for (int i = 0; i < 5; i++)
        dynarr_push(&arr, &i);

    assert(dynarr_size(&arr) == 5);
    assert(!dynarr_empty(&arr));

    /* get */
    for (size_t i = 0; i < dynarr_size(&arr); i++) {
        int v = *(int *)dynarr_get(&arr, i);
        assert(v == (int)i);
    }

    /* back */
    assert(*(int *)dynarr_back(&arr) == 4);

    /* pop */
    dynarr_pop(&arr);
    dynarr_pop(&arr);
    assert(dynarr_size(&arr) == 3);
    assert(*(int *)dynarr_back(&arr) == 2);

    /* insert */
    int x = 99;
    dynarr_insert(&arr, 1, &x);
    assert(*(int *)dynarr_get(&arr, 1) == 99);

    /* remove */
    dynarr_remove(&arr, 1);
    assert(*(int *)dynarr_get(&arr, 1) == 1);

    /* resize */
    dynarr_resize(&arr, 10);
    assert(dynarr_size(&arr) == 10);

    dynarr_resize(&arr, 2);
    assert(dynarr_size(&arr) == 2);

    /* reserve + shrink */
    dynarr_reserve(&arr, 32);
    dynarr_shrink_to_fit(&arr); /* sanity, not strict */

    /* find */
    int key = 1;
    ssize_t idx = dynarr_find(&arr, &key);
    assert(idx == 1);

    /* copy */
    DynArr copy;
    dynarr_copy(&copy, &arr);
    assert(dynarr_size(&copy) == dynarr_size(&arr));
    assert(*(int *)dynarr_get(&copy, 0) ==
           *(int *)dynarr_get(&arr, 0));

    /* move */
    DynArr moved;
    dynarr_move(&moved, &copy);
    assert(dynarr_size(&copy) == 0);
    assert(dynarr_size(&moved) == dynarr_size(&arr));

    /* swap */
    dynarr_swap(&arr, &moved);
    assert(*(int *)dynarr_get(&arr, 0) ==
           *(int *)dynarr_get(&moved, 0));

    /* extend */
    dynarr_extend(&arr, &moved);
    assert(dynarr_size(&arr) ==
           2 * dynarr_size(&moved));

    /* sort */
    dynarr_sort(&arr, int_cmp);
    for (size_t i = 1; i < dynarr_size(&arr); i++) {
        int a = *(int *)dynarr_get(&arr, i - 1);
        int b = *(int *)dynarr_get(&arr, i);
        assert(a <= b);
    }

    /* clear */
    dynarr_clear(&arr);
    assert(dynarr_empty(&arr));

    /* free */
    dynarr_free(&arr);
    dynarr_free(&moved);

    printf("All DynArr tests passed.\n");
    return 0;
}
