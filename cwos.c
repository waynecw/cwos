#include "uart.h"



void cwos_boot() 
{
	uart_init();

	uart0_send("=== CWOS ===\n");

	while (1) {
	}
}
