#ifndef __IRQ__
#define __IRQ__

#include <memory.h>

#define IRQ_COUNT		32

#define VIC_BASE		0x10140000
#define PIC_BASE		VIC_BASE
#define PIC_IRQ_STATUS		( PIC_BASE + 0 )
#define PIC_INT_ENABLE		( PIC_BASE + 0x10 )

#define PIC_UART0_BIT		0x00001000


#define trigger_swi() \
	__asm__ __volatile__ ( \
		"swi #0x55\n" \
	); \

void irq_enable();
void irq_disable();
void request_irq(int, void (*)(void), void *);

void handler_swi();


#endif
