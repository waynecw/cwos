#include "uart.h"


void cwos_boot() 
{
	uart_init();

	uart0_send("Hello CWOS World\n");

	while (1) {
	}
}
