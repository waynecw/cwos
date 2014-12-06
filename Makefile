CROSS_COMPILE := arm-none-eabi-

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
AS := $(CROSS_COMPILE)as
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy
NM := $(CROSS_COMPILE)nm

TARGET := versatilepb

OS := cwos

CFLAGS := -nostdinc -mcpu=arm926ej-s -g -Iinclude -fno-builtin
ASFLAGS := -mcpu=arm926ej-s -g 

LDSCRIPT := cwos.ld
LDFLAGS := -L/usr/lib/gcc/arm-none-eabi/4.8.2 -lgcc

QEMU := qemu-system-arm
QEMU_FLAGS := -nographic -M $(TARGET) -m 128M -kernel $(OS).img -gdb tcp::26000 -serial mon:stdio

OBJS := boot.o 

include device/build.mk
include kernel/build.mk
include lib/build.mk

$(OS): $(OBJS)
	$(LD) -o $@.elf -T $(LDSCRIPT) $(OBJS) $(LDFLAGS)
	$(OBJDUMP) -D $@.elf > $@.asm
	$(OBJCOPY) -O binary $@.elf $@.img
	$(NM) $@.elf | sort > $@.map


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
	rm -f $(OBJS)
	rm -rf *.elf *.asm *.img *.map
