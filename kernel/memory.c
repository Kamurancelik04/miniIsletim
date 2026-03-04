#include "../include/memory.h"

#define MAX_BLOCKS (HEAP_SIZE / BLOCK_SIZE)

static uint8_t memory_map[MAX_BLOCKS];
static uint32_t total_blocks;
static uint32_t used_blocks;

void memory_init() {
    total_blocks = MAX_BLOCKS;
    used_blocks = 0;
    int i;
    for (i = 0; i < MAX_BLOCKS; i++) {
        memory_map[i] = 0;
    }
}

void* kmalloc(size_t size) {
    uint32_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    uint32_t consecutive = 0;
    uint32_t start_block = 0;
    uint32_t i;

    for (i = 0; i < total_blocks; i++) {
        if (memory_map[i] == 0) {
            if (consecutive == 0) start_block = i;
            consecutive++;
            if (consecutive == blocks_needed) {
                uint32_t j;
                for (j = start_block; j < start_block + blocks_needed; j++) {
                    memory_map[j] = 1;
                }
                used_blocks += blocks_needed;
                return (void*)(HEAP_START + start_block * BLOCK_SIZE);
            }
        } else {
            consecutive = 0;
        }
    }
    return NULL;
}

void kfree(void* ptr) {
    if (ptr == NULL) return;
    uint32_t block = ((uint32_t)ptr - HEAP_START) / BLOCK_SIZE;
    if (block < total_blocks && memory_map[block]) {
        memory_map[block] = 0;
        used_blocks--;
    }
}

void memory_copy(const char* source, char* dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        dest[i] = source[i];
    }
}

void memory_set(void* dest, uint8_t val, uint32_t len) {
    uint8_t* d = (uint8_t*)dest;
    uint32_t i;
    for (i = 0; i < len; i++) {
        d[i] = val;
    }
}

int string_length(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int string_compare(const char* s1, const char* s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

void string_copy(const char* src, char* dest) {
    while (*src) {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
}

void int_to_string(int num, char* str) {
    int i = 0;
    int is_negative = 0;
    char temp[12];

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }

    int j = 0;
    if (is_negative) str[j++] = '-';
    while (i > 0) {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
}