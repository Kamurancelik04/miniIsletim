# 🖥️ KamuranOS - Mini İşletim Sistemi

x86 mimarisinde Assembly ve C ile yazılmış eğitim amaçlı mini işletim sistemi.

## ✨ Özellikler

- 🔧 **Özel Bootloader** - BIOS'tan boot eden 512 byte'lık bootloader
- 🛡️ **Protected Mode** - 32-bit protected mode desteği
- 🖥️ **VGA Text Mode** - Renkli ekran çıktısı (16 renk)
- ⌨️ **Klavye Sürücüsü** - PS/2 klavye desteği
- 💾 **Bellek Yönetimi** - 1MB heap ile basit blok bellek yönetimi
- 🐚 **Shell** - Komut satırı arayüzü

## 📋 Komutlar

| Komut | Açıklama |
|-------|----------|
| `help` | Yardım mesajını gösterir |
| `clear` | Ekranı temizler |
| `about` | Sistem hakkında bilgi verir |
| `memory` | Bellek durumunu gösterir |
| `echo <mesaj>` | Yazılanı tekrarlar |
| `color` | Renk testini gösterir |
| `reboot` | Sistemi yeniden başlatır |

## 🛠️ Gereksinimler

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install nasm qemu-system-x86 build-essential
# Cross compiler kurulumu
sudo apt install gcc-i686-linux-gnu
```

### macOS
```bash
brew install nasm qemu
brew install i686-elf-gcc  # veya crosstools ile derleyin
```

## 🚀 Derleme ve Çalıştırma

```bash
# Projeyi derle
make

# QEMU ile çalıştır
make run

# Temizle
make clean

# Debug modunda çalıştır
make debug
```

## 📁 Proje Yapısı

```
miniIsletim/
├── boot/
│   └── boot.asm          # Bootloader (512 byte)
├── kernel/
│   ├── kernel_entry.asm  # Kernel giriş noktası
│   ├── kernel.c          # Ana kernel
│   ├── screen.c          # VGA ekran sürücüsü
│   ├── keyboard.c        # Klavye sürücüsü
│   ├── memory.c          # Bellek yönetimi
│   ├── shell.c           # Komut satırı
│   └── idt.c             # Interrupt Descriptor Table
├── include/
│   ├── types.h           # Tip tanımları
│   ├── screen.h          # Ekran header
│   ├── keyboard.h        # Klavye header
│   ├── memory.h          # Bellek header
│   ├── shell.h           # Shell header
│   └── idt.h             # IDT header
├── linker.ld             # Linker scripti
├── Makefile              # Build sistemi
└── README.md             # Bu dosya
```

## 📖 Nasıl Çalışır?

1. **BIOS** bilgisayarı başlatır ve boot sektörünü (ilk 512 byte) RAM'e yükler
2. **Bootloader** kernel'i diskten okur ve Protected Mode'a geçer
3. **Kernel** başlatılır: bellek, klavye ve IDT kurulur
4. **Shell** açılır ve kullanıcıdan komut bekler

## 📜 Lisans

MIT License - Kamurancelik04
