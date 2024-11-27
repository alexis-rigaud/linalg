/* tests.h
   (c) Matthew Drury, 2015
*/
#include <stdbool.h>

struct test {
    bool (*test_f)();
    char* name;
};

void run_all(void);
