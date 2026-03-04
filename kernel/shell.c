#include "../include/shell.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/types.h"

static void print_banner() {
    screen_print_color("  _  __                                   ___  ____  \n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_color(" | |/ /__ _ _ __ ___  _   _ _ __ __ _ _ __/ _ \\ ___| \n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_color(" | ' // _` | '_ ` _ \\| | | | '__/ _` | '_\\ | | \\___ \\ \n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_color(" | . \\ (_| | | | | | | |_| | | | (_| | | | |_| |___) |\n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_color(" |_|\\_\\__,_|_| |_| |_|\\__,_|_|  \\__,_|_|  \\___/|____/ \n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_newline();
    screen_print_color(" KamuranOS v1.0 - Mini Isletim Sistemi\n", COLOR_YELLOW, COLOR_BLACK);
    screen_print_color(" Yardim icin 'help' yazin.\n\n", COLOR_LIGHT_GREY, COLOR_BLACK);
}

static void print_help() {
    screen_print_color("  Kullanilabilir Komutlar:\n", COLOR_YELLOW, COLOR_BLACK);
    screen_print_color("  ----------------------\n", COLOR_YELLOW, COLOR_BLACK);
    screen_print_color("  help     ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Bu yardim mesajini gosterir\n");
    screen_print_color("  clear    ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Ekrani temizler\n");
    screen_print_color("  about    ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Sistem hakkinda bilgi\n");
    screen_print_color("  memory   ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Bellek durumunu gosterir\n");
    screen_print_color("  echo     ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Yazdiginizi tekrarlar (echo merhaba)\n");
    screen_print_color("  color    ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Renk testini gosterir\n");
    screen_print_color("  reboot   ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print("- Sistemi yeniden baslatir\n");
    screen_print_newline();
}

static void print_about() {
    screen_print_color("  ================================\n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_color("  KamuranOS v1.0\n", COLOR_YELLOW, COLOR_BLACK);
    screen_print_color("  ================================\n", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print("  Gelistirici : Kamurancelik04\n");
    screen_print("  Mimari      : x86 (32-bit Protected Mode)\n");
    screen_print("  Dil         : Assembly (NASM) + C\n");
    screen_print("  Bootloader  : Ozel (512 byte)\n");
    screen_print("  Ozellikler  : VGA Text Mode, Klavye, Shell\n");
    screen_print("  Lisans      : MIT\n");
    screen_print_newline();
}

static void print_memory_info() {
    screen_print_color("  Bellek Bilgisi:\n", COLOR_YELLOW, COLOR_BLACK);
    screen_print("  Heap Baslangic : 0x100000 (1MB)\n");
    screen_print("  Heap Boyutu    : 1 MB\n");
    screen_print("  Blok Boyutu    : 4 KB\n");
    screen_print("  Toplam Blok    : 256\n");
    screen_print_newline();
}

static void print_color_test() {
    screen_print_color("  Renk Testi:\n", COLOR_YELLOW, COLOR_BLACK);
    screen_print_color("  SIYAH ", COLOR_BLACK, COLOR_LIGHT_GREY);
    screen_print_color(" MAVI ", COLOR_LIGHT_BLUE, COLOR_BLACK);
    screen_print_color(" YESIL ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    screen_print_color(" CYAN ", COLOR_LIGHT_CYAN, COLOR_BLACK);
    screen_print_color(" KIRMIZI ", COLOR_LIGHT_RED, COLOR_BLACK);
    screen_print_color(" MAGENTA ", COLOR_LIGHT_MAGENTA, COLOR_BLACK);
    screen_print_color(" SARI ", COLOR_YELLOW, COLOR_BLACK);
    screen_print_color(" BEYAZ ", COLOR_WHITE, COLOR_BLACK);
    screen_print_newline();
    screen_print_newline();
}

static int starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str != *prefix) return 0;
        str++;
        prefix++;
    }
    return 1;
}

static void do_reboot() {
    screen_print_color("  Sistem yeniden baslatiliyor...\n", COLOR_LIGHT_RED, COLOR_BLACK);
    // Triple fault ile reboot
    uint8_t good = 0x02;
    while (good & 0x02)
        good = port_byte_in(0x64);
    port_byte_out(0x64, 0xFE);
}

void shell_execute_command(const char* command) {
    if (string_length(command) == 0) {
        return;
    } else if (string_compare(command, "help") == 0) {
        print_help();
    } else if (string_compare(command, "clear") == 0) {
        screen_clear();
        print_banner();
    } else if (string_compare(command, "about") == 0) {
        print_about();
    } else if (string_compare(command, "memory") == 0) {
        print_memory_info();
    } else if (string_compare(command, "color") == 0) {
        print_color_test();
    } else if (string_compare(command, "reboot") == 0) {
        do_reboot();
    } else if (starts_with(command, "echo ")) {
        screen_print("  ");
        screen_print(command + 5);
        screen_print_newline();
    } else {
        screen_print_color("  Bilinmeyen komut: '\", COLOR_LIGHT_RED, COLOR_BLACK);
        screen_print_color(command, COLOR_LIGHT_RED, COLOR_BLACK);
        screen_print_color("'. 'help' yazin.\n", COLOR_LIGHT_RED, COLOR_BLACK);
    }
}

void shell_init() {
    screen_clear();
    print_banner();
}

void shell_run() {
    char input[256];

    while (1) {
        screen_print_color("kamuran", COLOR_LIGHT_GREEN, COLOR_BLACK);
        screen_print_color("@", COLOR_WHITE, COLOR_BLACK);
        screen_print_color("os", COLOR_LIGHT_BLUE, COLOR_BLACK);
        screen_print_color(" $ ", COLOR_WHITE, COLOR_BLACK);

        keyboard_read_line(input, 256);
        shell_execute_command(input);
    }
}