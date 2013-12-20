#ifndef __UART__
#define __UART__

void uart_init();
void uart0_send(const char *str);
void uart0_putchar(const char c);

void uart0_irq_handler();

#endif
