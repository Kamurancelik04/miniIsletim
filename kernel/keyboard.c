#include "../include/keyboard.h"
#include "../include/screen.h"

static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

void keyboard_init() {
    // Klavye tamponunu temizle
    while (port_byte_in(KEYBOARD_STATUS_PORT) & 1) {
        port_byte_in(KEYBOARD_DATA_PORT);
    }
}

char keyboard_read_char() {
    uint8_t scancode;

    while (1) {
        if (port_byte_in(KEYBOARD_STATUS_PORT) & 1) {
            scancode = port_byte_in(KEYBOARD_DATA_PORT);

            // Sadece key press (key release degil)
            if (scancode < 0x80) {
                if (scancode == 0x0E) return '\b';  // Backspace
                if (scancode == 0x1C) return '\n';   // Enter
                if (scancode < sizeof(scancode_to_ascii)) {
                    char c = scancode_to_ascii[scancode];
                    if (c != 0) return c;
                }
            }
        }
    }
}

void keyboard_read_line(char* buffer, int max_length) {
    int i = 0;
    char c;

    while (i < max_length - 1) {
        c = keyboard_read_char();

        if (c == '\n') {
            buffer[i] = '\0';
            screen_print_newline();
            return;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                screen_print_backspace();
            }
        } else {
            buffer[i] = c;
            i++;
            screen_print_char(c);
        }
    }
    buffer[i] = '\0';
    screen_print_newline();
}