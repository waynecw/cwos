#include <uart.h>
#include <irq.h>



void cwos_boot() 
{
	uart_init();

	uart0_send("Hello CWOS World\n");
	uart0_send("Press any key to see echo:\n");

	irq_enable();

	while (1) {
	}
}
