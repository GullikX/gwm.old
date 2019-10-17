#include "gwm.h"

int main() {
    int* numbers = ecalloc(5, sizeof(*numbers));
    printf("%zu\n", sizeof(*numbers));
}
