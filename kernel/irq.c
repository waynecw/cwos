#include <irq.h>

void irq_enable()
{
	__asm__ __volatile__ (
		"mrs r0, cpsr\n"
		"bic r0, r0, #0x80\n"
		"msr cpsr, r0\n"
	);
}

void irq_disable()
{
	__asm__ __volatile__ (
		"mrs r0, cpsr\n"
		"orr r0, #0xc0\n"
		"msr cpsr, r0\n"
	);
}

void handler_swi()
{
	uart0_send("In SWI!\n");
	irq_enable();
}

void handler_irq()
{
	volatile unsigned int *reg = (unsigned int *) PIC_IRQ_STATUS;

	switch (*reg) {
		case PIC_UART0_BIT:
			uart0_irq_handler();
		break;

	}
#if 0
	__asm__ __volatile__ (
		"mrs r0, cpsr\n"
		"bic r0, r0, #0x1F\n"
		"orr r0, r0, #0x12\n" // enter SVC mode
		"msr cpsr, r0\n"
		"subs pc, lr, #4\n"
	);
#endif
}
