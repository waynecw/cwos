ENTRY(_start)
SECTIONS
{
	. = 0x00000000;  /* QEMU versatilepb start address */
	.text : { 
		boot.o (interrupt_vector)
		*(.text)
	}
	isr_end = .;
	.data : { *(.data) }
	.bss : { *(.bss COMMON) }
	. = ALIGN(8);
	. = . + 0x1000;
	kernel_stack_top = .;
}
