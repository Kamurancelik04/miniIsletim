# KamuranOS - Mini Isletim Sistemi
# Makefile

CC = i686-elf-gcc
LD = i686-elf-ld
ASM = nasm

CFLAGS = -ffreestanding -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
LDFLAGS = -T linker.ld --oformat binary

KERNEL_SOURCES = kernel/kernel.c kernel/screen.c kernel/keyboard.c kernel/memory.c kernel/shell.c kernel/idt.c
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)

all: os-image.bin

# Bootloader derleme
boot/boot.bin: boot/boot.asm
	$(ASM) -f bin $< -o $@

# Kernel giris noktasi
kernel/kernel_entry.o: kernel/kernel_entry.asm
	$(ASM) -f elf32 $< -o $@

# C dosyalarini derleme
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Kernel binary
kernel.bin: kernel/kernel_entry.o $(KERNEL_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

# OS imaji olustur
os-image.bin: boot/boot.bin kernel.bin
	cat $^ > $@
	# 30 sektore tamamla (15360 byte)
	truncate -s 15360 $@

# QEMU ile calistir
run: os-image.bin
	qemu-system-i386 -fda os-image.bin

# Debug modunda calistir
debug: os-image.bin
	qemu-system-i386 -fda os-image.bin -s -S &
	gdb -ex "target remote localhost:1234" -ex "set architecture i386"

# Temizlik
clean:
	rm -f boot/*.bin kernel/*.o *.bin *.o

.PHONY: all run debug clean
