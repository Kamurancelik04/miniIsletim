#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init();
char keyboard_read_char();
void keyboard_read_line(char* buffer, int max_length);

// Port I/O
uint8_t port_byte_in(uint16_t port);
void port_byte_out(uint16_t port, uint8_t data);

#endif