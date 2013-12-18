CROSS_COMPILE := arm-none-eabi-

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
AS := $(CROSS_COMPILE)as
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy

TARGET := versatilepb

OS := cwos

CFLAGS := -nostdinc -mcpu=arm926ej-s -g -Iinclude
ASFLAGS := -mcpu=arm926ej-s -g 

LDSCRIPT := cwos.ld

QEMU := qemu-system-arm
QEMU_FLAGS := -nographic -M $(TARGET) -m 128M -kernel $(OS).img -gdb tcp::26000 -serial mon:stdio

OBJS := boot.o cwos.o 

include device/build.mk

$(OS): $(OBJS)
	$(LD) -o $@.elf -T $(LDSCRIPT) $(OBJS)
	$(OBJDUMP) -D $@.elf > $@.asm
	$(OBJCOPY) -O binary $@.elf $@.img


qemu: $(OS)
	$(QEMU) $(QEMU_FLAGS)

qemu-debug: $(OS)
	@echo "Attach gdb through tcp::26000"
	@$(QEMU) $(QEMU_FLAGS) -S
 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	find . -name '*.o' -exec rm -f {} \;
	rm -rf *.elf *.asm *.img
