#include "../include/types.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/shell.h"
#include "../include/idt.h"

void kernel_main() {
    // Alt sistemleri baslat
    memory_init();
    keyboard_init();
    idt_init();

    // Shell'i baslat
    shell_init();
    shell_run();
}