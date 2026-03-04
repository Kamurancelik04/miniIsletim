#include "../include/idt.h"

idt_entry_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void idt_set_gate(int n, uint32_t handler) {
    idt[n].low_offset = (uint16_t)(handler & 0xFFFF);
    idt[n].selector = 0x08;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = (uint16_t)((handler >> 16) & 0xFFFF);
}

void idt_init() {
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_entry_t) - 1;
    memory_set(&idt, 0, sizeof(idt_entry_t) * IDT_ENTRIES);
    __asm__ volatile("lidt (%0)" : : "r" (&idt_reg));
}