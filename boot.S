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
	adr r0, _start
	ldr r1, =0
	cmp r0, r1
	beq setup_stack

relocation:
	ldr r2, =_start
	ldr r3, =isr_end
	sub r2, r3, r2
	add r2, r0, r2

relocation_loop:
	ldmia r0!, {r3-r10}
	stmia r1!, {r3-r10}
	cmp r0, r2
	ble relocation_loop
	ldr pc, =0

setup_stack:
	ldr sp, =kernel_stack_top
	bl cwos_boot 
