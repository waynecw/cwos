ENTRY(_start)
SECTIONS
{
	. = 0x00000000;  /* QEMU versatilepb start address */
	.text : { 
		_text = .;
		boot.o (interrupt_vector)
		*(.text)
		_etext = .;
	}

	.rodata : {
		_rodata = .;
		*(.rodata)
		_erodata = .;
	}

	. = ALIGN(0x4000);  /* TTB requires to align to 16K */
	_ttb_start = .;
	. += 0x4000;  /* TTB requires 16K memory */
	_ettb_start = .;

	.data : { 
		_data = .;
		*(.data) 
		_edata = .;
	}
	.bss : {
		_bss = .;
		*(.bss)
		*(COMMON)
		_ebss = .;
	} 

	. = ALIGN(0x1000);

	kernel_stack_top = .;
	. += 0x1000;
	kernel_stack_start = .;
	. += 0x1000;
	irq_stack_start = .;
}
