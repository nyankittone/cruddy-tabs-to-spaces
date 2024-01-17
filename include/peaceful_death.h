#include <stddef.h>

#define ALLOC_OUT_OF_MEM_EXIT 1

// don't know a better way to do this tbh
#define INFO_ERROR_TEXT "\33[1;97minfo:\33[m"
#define NORMAL_ERROR_TEXT "\33[1;95merror:\33[m"
#define FATAL_ERROR_TEXT "\33[1;91mfatal:\33[m"

void die(int exit_code, const char *death_message, ...);
void *reallocOrDie(void *old_block, size_t new_allocated_size);
void *mallocOrDie(size_t allocated_size);

