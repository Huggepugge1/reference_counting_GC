#include "refmem.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct cell {
    struct cell *cell;
    char *string;
    int i;
};

void test_array() {
    const size_t ARRAY_SIZE = 10000;
    size_t **array = allocate_array(ARRAY_SIZE, sizeof(size_t *), NULL, 1);
    retain(array);

    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        array[i] = allocate(sizeof(size_t), NULL, 0);
        retain(array[i]);
        *array[i] = i;
    }

    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        assert(*array[i] == i);
    }

    printf("%lu\n",
           sizeof(size_t) * ARRAY_SIZE + sizeof(size_t *) * ARRAY_SIZE);
    release(array);
}

void cell_destructor(obj *c) { release(((struct cell *)c)->cell); }

int main(void) {
    struct cell *c = allocate(sizeof(struct cell), NULL, 1);
    retain(c);

    set_cascade_limit(1000000);

    c->cell = allocate(sizeof(struct cell), NULL, 1);
    c->cell->i = 42;
    retain(c->cell);
    c->cell = NULL;

    release(c->cell);
    c->cell = allocate(sizeof(struct cell), NULL, 1);
    c->cell->i = 40;
    retain(c->cell);

    c->cell->cell = NULL;

    test_array();
    test_array();
    shutdown();

    return 0;
}
