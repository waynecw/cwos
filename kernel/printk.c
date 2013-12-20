#include <uart.h>
#include <stdarg.h>

static inline unsigned int getuint(va_list *va)
{
	return va_arg(*va, unsigned int);
}

static inline int getint(va_list *va)
{
	return va_arg(*va, int);
}


static inline void printdigit(void (*putch)(const char), int d)
{
	const char *digits = "01234567789abcdef";
	putch(digits[d]);
}

static inline void printnum(
		void (*putch)(const char), 
		unsigned long long num,
		int base
		)
{
	int d;
	long long n = (long long) num;

	/* only supports 2 <= base <= 16 */
	if (base < 2 || base > 16) {
		return;
	}

	/* recursive, print least significant digit first */
	if (num > base)
		printnum(putch, num / base, base);

	printdigit(putch, num % base);
}


void vprintfmt(void (*putch)(const char), const char *fmt, va_list *va)
{
	char ch;
	unsigned long long num;
	int base;

	while (1) {
		ch = *(unsigned char *) fmt++;
		switch (ch)
		{
			case '\0':
				return;

			case '%':
				ch = *(unsigned char *) fmt++;
				switch (ch) {
					case 'd':
						num = getint(va);
						if ((long long) num < 0) {
							putch('-');
							num = (long long) num * -1;
						}
						base = 10;
						printnum(putch, num, base);
						break;

					case 'x':
						num = getuint(va);
						base = 16;
						printnum(putch, num, base);
				}

				break;

			default:
				putch(ch);
				break;
		}

	}
}

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
