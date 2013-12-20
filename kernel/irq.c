#include <irq.h>
#include <uart.h>

void (*pl190_isr_vectors[IRQ_COUNT])(void) = 
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	uart0_irq_handler,
	0, 0, 0
};


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
	printk("In SWI!\n");
	irq_enable();
}

void handler_irq()
{
	unsigned int reg_irq_status = *((volatile unsigned int *) PIC_IRQ_STATUS);
	int irq_n = 0;

	while (reg_irq_status >>= 1) 
		irq_n++;

	pl190_isr_vectors[irq_n]();

}
