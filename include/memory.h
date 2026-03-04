#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define HEAP_START 0x100000
#define HEAP_SIZE  0x100000  // 1MB heap
#define BLOCK_SIZE 4096      // 4KB bloklar

void memory_init();
void* kmalloc(size_t size);
void kfree(void* ptr);
void memory_copy(const char* source, char* dest, int nbytes);
void memory_set(void* dest, uint8_t val, uint32_t len);
int string_length(const char* str);
int string_compare(const char* s1, const char* s2);
void string_copy(const char* src, char* dest);
void int_to_string(int num, char* str);

#endif