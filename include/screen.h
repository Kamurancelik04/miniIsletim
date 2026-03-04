#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

#define VIDEO_MEMORY 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Renkler
#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GREY 0x7
#define COLOR_DARK_GREY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

void screen_clear();
void screen_print(const char* str);
void screen_print_color(const char* str, uint8_t foreground, uint8_t background);
void screen_print_char(char c);
void screen_print_char_color(char c, uint8_t foreground, uint8_t background);
void screen_set_cursor(int row, int col);
void screen_print_newline();
void screen_print_backspace();
void screen_scroll();
int get_cursor_offset();
void set_cursor_offset(int offset);

#endif