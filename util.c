#include "gwm.h"

void* ecalloc(size_t nItems, size_t itemSize) {
    void* pointer = calloc(nItems, itemSize);
    if (!pointer) {
        fprintf(stderr, "ecalloc: Failed to allocate %zux%zu bytes.\n", nItems, itemSize);
        exit(EXIT_FAILURE);
    }
    return pointer;
}
