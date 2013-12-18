.code 32
.section interrupt_vector
.global _start
_start:
	b reset_handler
	b .
	b .
	b .
	b .
	b .
	b .
	b .

reset_handler:
	ldr sp, =kernel_stack_top
	swi 0
	bl cwos_boot 
