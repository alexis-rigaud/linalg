/* util.c
   (c) Matthew Drury, 2015
*/
#include <stdlib.h>
#include "util.h"
#include "errors.h"

void check_memory(void* mem) {
    if(mem == NULL) {
        raise_memory_allocation_error();
    }
}
