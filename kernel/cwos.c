#include <uart.h>
#include <irq.h>
#include <printk.h>
#include <memory.h>

void cwos_boot() 
{
	mmu_init();
}

void cwos_main()
{
	uart_init();

	printk("Hello CWOS %d.%x World\n", 0, 0x1);
	printk("Press any key to see echo: ");

	irq_enable();

	cwshell();
}
