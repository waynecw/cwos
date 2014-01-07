#include <uart.h>
#include <stdarg.h>
#include <types.h>

static void __putchar(const char buf)
{
	uart0_putchar(buf);
}

void printk(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vprintfmt(__putchar, fmt, &va);
	va_end(va);
}
