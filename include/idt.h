#ifndef IDT_H
#define IDT_H

#include "types.h"

#define IDT_ENTRIES 256

typedef struct {
    uint16_t low_offset;
    uint16_t selector;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

void idt_init();
void idt_set_gate(int n, uint32_t handler);

#endif