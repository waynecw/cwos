#include <uart.h>
#include <irq.h>



void cwos_boot() 
{
	uart_init();

	uart0_send("Hello CWOS World\n");

	irq_enable();

	trigger_swi();

	while (1) {
	}
}
