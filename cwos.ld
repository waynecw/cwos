ENTRY(_start)
SECTIONS
{
	. = 0x00010000;  /* QEMU versatilepb start address */
	.text : { 
		boot.o (interrupt_vector)
		*(.text)
	}
	.data : { *(.data) }
	.bss : { *(.bss COMMON) }
	. = ALIGN(8);
	. = . + 0x1000;
	kernel_stack_top = .;
}
