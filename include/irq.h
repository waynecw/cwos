#ifndef __IRQ__
#define __IRQ__

#define IRQ_COUNT	32


#define trigger_swi() \
	__asm__ __volatile__ ( \
		"swi #0x55\n" \
	); \

void irq_enable();
void irq_disable();

void handler_swi();


#endif
