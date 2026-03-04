[org 0x7c00]
KERNEL_OFFSET equ 0x1000

    ; BIOS boot sector
    mov [BOOT_DRIVE], dl

    ; Stack kurulumu
    mov bp, 0x9000
    mov sp, bp

    ; Ekrana hosgeldin mesaji
    mov bx, MSG_REAL_MODE
    call print_string

    ; Kerneli diskten yukle
    call load_kernel

    ; Protected mode'a gec
    call switch_to_pm

    jmp $

; ============================================
; Ekrana yazi yazdirma (Real Mode)
; ============================================
print_string:
    pusha
    mov ah, 0x0e
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    int 0x10
    inc bx
    jmp .loop
.done:
    popa
    ret

; ============================================
; Disk okuma
; ============================================
disk_load:
    pusha
    push dx

    mov ah, 0x02    ; BIOS read sector
    mov al, dh      ; Sector sayisi
    mov cl, 0x02    ; Baslangic sector (1=boot sector, 2=ilk sector)
    mov ch, 0x00    ; Cylinder 0
    mov dh, 0x00    ; Head 0

    int 0x13
    jc disk_error

    pop dx
    cmp al, dh
    jne sectors_error
    popa
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp disk_loop

sectors_error:
    mov bx, SECTORS_ERROR_MSG
    call print_string

disk_loop:
    jmp $

DISK_ERROR_MSG:
    db "Disk okuma hatasi!", 0
SECTORS_ERROR_MSG:
    db "Yanlis sector sayisi!", 0

; ============================================
; Kerneli yukle
; ============================================
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET
    mov dh, 30          ; 30 sector oku
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

; ============================================
; GDT (Global Descriptor Table)
; ============================================
gdt_start:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; ============================================
; Protected Mode'a gecis
; ============================================
[bits 16]
switch_to_pm:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL_OFFSET

    jmp $

MSG_REAL_MODE:
    db "Real Mode'da baslatildi...", 0
MSG_LOAD_KERNEL:
    db "Kernel yukleniyor...", 0

BOOT_DRIVE:
    db 0

    ; Boot sector 512 byte olmali
    times 510-($-$$) db 0
    dw 0xAA55
