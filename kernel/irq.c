#include <irq.h>
#include <uart.h>
#include <types.h>
#include <memory.h>

static void (*irq_vectors[IRQ_COUNT])(void);

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

void request_irq(int irqno, void (*func)(void), void *flag) 
{
	irq_vectors[irqno] = func;
}

void handler_irq()
{
	uint32_t reg_irq_status = *((volatile uint32_t *) PIC_IRQ_STATUS);
	int irqno = 0;

	do {
		if (reg_irq_status & 1) 
			irq_vectors[irqno]();
		irqno++;
	} while (reg_irq_status >>= 1);
}
