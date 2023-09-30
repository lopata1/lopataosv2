CC := i686-elf-gcc
LD := i686-elf-ld
AS := nasm
CFLAGS := -ffreestanding -nostdlib -m32 -g -c -I./include/
ASMFLAGS := -f elf
LDFLAGS := -T linker.ld --oformat binary

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

CSOURCES := kernel.c ints/interrupts.c output.c globals.c prompt.c string.c games/pong/pong.c games/snake/snake.c graphics.c characters.c math.c
ASMSOURCES := ints/isr_error.asm ints/stubs.asm memory.asm

COBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(CSOURCES))
ASMOBJECTS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASMSOURCES))

all: $(ASMOBJECTS) $(COBJECTS) $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kentry.o
	$(LD) $(LDFLAGS) $(BUILD_DIR)/kentry.o $(ASMOBJECTS) $(COBJECTS) -o $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin > $(BIN_DIR)/os.bin


$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/bootloader/bootloader.asm
	$(AS) -f bin $< -o $@

$(BUILD_DIR)/kentry.o: $(SRC_DIR)/bootloader/kentry.asm
	$(AS) -f elf $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	$(AS) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@	
	

floppy: all
	dd if=$(BIN_DIR)/os.bin of=$(BIN_DIR)/os.img bs=512 count=2880

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.bin
	rm -f $(BUILD_DIR)/ints/*.o
	rm -f $(BUILD_DIR)/bootloader/*.o
	rm -f $(BIN_DIR)/*

.PHONY: all