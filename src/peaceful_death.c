#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <peaceful_death.h>

static const char OUT_OF_MEM_MESSAGE[] = "Out of memory!";

void die(int exit_code, const char *death_message, ...) {
    va_list message_list;
    va_start(message_list,death_message);

    fputs("\33[1;91mfatal:\33[m ",stderr);
    vfprintf(stderr,death_message,message_list);
    fputc('\n',stderr);

    va_end(message_list);
    exit(exit_code);
}

void *reallocOrDie(void *old_block, size_t new_allocated_size) {
    void *new_block = realloc(old_block,new_allocated_size);
    if(!new_block) die(ALLOC_OUT_OF_MEM_EXIT,OUT_OF_MEM_MESSAGE);

    return new_block;
}

void *mallocOrDie(size_t new_allocated_size) {
    void *new_block = malloc(new_allocated_size);
    if(!new_block) die(ALLOC_OUT_OF_MEM_EXIT,OUT_OF_MEM_MESSAGE);

    return new_block;
}

