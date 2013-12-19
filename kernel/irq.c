#include <irq.h>

void irq_enable()
{
	__asm__ __volatile__ (
		"mrs r0, cpsr\n"
		"bic r0, #0xc0\n"
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
}

void handler_irq()
{
	uart0_send("In ARM Core IRQ!\n");
}
