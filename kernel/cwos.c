#include "uart.h"


#define PGSIZE		0x1000

extern unsigned int kernel_mem_start;
extern unsigned int kernel_mem_end;

void *boot_malloc(unsigned int size)
{
	static char *nextfree;
	char *result;

	if (!nextfree) 
		nextfree = (char *) &kernel_mem_start;

	if (!size)
		return (void *) nextfree;

	result = nextfree;
	nextfree += size;

	return result;
}


void cwos_boot() 
{
	char *strbuf;

	uart_init();

	uart0_send("World\n");

	while (1) {
	}
}
