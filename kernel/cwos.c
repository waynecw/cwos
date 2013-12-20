#include <uart.h>
#include <irq.h>
#include <printk.h>


void cwos_boot() 
{
	uart_init();

	printk("Hello CWOS %d.%x World\n", 0, 0x1);
	printk("Press any key to see echo: ");

	irq_enable();

	while (1) {
	}
}
