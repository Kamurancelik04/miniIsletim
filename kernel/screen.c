#include "../include/screen.h"
#include "../include/types.h"

static int cursor_row = 0;
static int cursor_col = 0;

static uint8_t default_fg = COLOR_LIGHT_GREEN;
static uint8_t default_bg = COLOR_BLACK;

int get_cursor_offset() {
    return (cursor_row * MAX_COLS + cursor_col) * 2;
}

void set_cursor_offset(int offset) {
    cursor_row = (offset / 2) / MAX_COLS;
    cursor_col = (offset / 2) % MAX_COLS;
}

void screen_update_cursor() {
    uint16_t pos = cursor_row * MAX_COLS + cursor_col;
    port_byte_out(0x3D4, 0x0F);
    port_byte_out(0x3D5, (uint8_t)(pos & 0xFF));
    port_byte_out(0x3D4, 0x0E);
    port_byte_out(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void port_byte_out(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

uint8_t port_byte_in(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void screen_clear() {
    volatile char *video = (volatile char*)VIDEO_MEMORY;
    int i;
    for (i = 0; i < MAX_ROWS * MAX_COLS * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = (default_bg << 4) | default_fg;
    }
    cursor_row = 0;
    cursor_col = 0;
    screen_update_cursor();
}

void screen_scroll() {
    volatile char *video = (volatile char*)VIDEO_MEMORY;
    int i;

    if (cursor_row >= MAX_ROWS) {
        for (i = 0; i < (MAX_ROWS - 1) * MAX_COLS * 2; i++) {
            video[i] = video[i + MAX_COLS * 2];
        }
        for (i = (MAX_ROWS - 1) * MAX_COLS * 2; i < MAX_ROWS * MAX_COLS * 2; i += 2) {
            video[i] = ' ';
            video[i + 1] = (default_bg << 4) | default_fg;
        }
        cursor_row = MAX_ROWS - 1;
    }
}

void screen_print_char_color(char c, uint8_t fg, uint8_t bg) {
    volatile char *video = (volatile char*)VIDEO_MEMORY;
    uint8_t attr = (bg << 4) | fg;

    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\t') {
        cursor_col = (cursor_col + 4) & ~3;
    } else {
        int offset = (cursor_row * MAX_COLS + cursor_col) * 2;
        video[offset] = c;
        video[offset + 1] = attr;
        cursor_col++;
    }

    if (cursor_col >= MAX_COLS) {
        cursor_col = 0;
        cursor_row++;
    }

    screen_scroll();
    screen_update_cursor();
}

void screen_print_char(char c) {
    screen_print_char_color(c, default_fg, default_bg);
}

void screen_print_color(const char* str, uint8_t fg, uint8_t bg) {
    int i = 0;
    while (str[i] != '\0') {
        screen_print_char_color(str[i], fg, bg);
        i++;
    }
}

void screen_print(const char* str) {
    screen_print_color(str, default_fg, default_bg);
}

void screen_print_newline() {
    screen_print_char('\n');
}

void screen_print_backspace() {
    if (cursor_col > 0) {
        cursor_col--;
    } else if (cursor_row > 0) {
        cursor_row--;
        cursor_col = MAX_COLS - 1;
    }
    volatile char *video = (volatile char*)VIDEO_MEMORY;
    int offset = (cursor_row * MAX_COLS + cursor_col) * 2;
    video[offset] = ' ';
    video[offset + 1] = (default_bg << 4) | default_fg;
    screen_update_cursor();
}

void screen_set_cursor(int row, int col) {
    cursor_row = row;
    cursor_col = col;
    screen_update_cursor();
}
