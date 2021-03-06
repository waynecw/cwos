#include <stdarg.h>
#include <lib.h>
#include <types.h>

static inline uint32_t getuint(va_list *va)
{
	return va_arg(*va, uint32_t);
}

static inline int32_t getint(va_list *va)
{
	return va_arg(*va, int32_t);
}


static inline void printdigit(void (*putch)(const char), int d)
{
	const char *digits = "0123456789abcdef";
	putch(digits[d]);
}

static inline void printnum(
		void (*putch)(const char), 
		uint64_t num,
		int base
		)
{
	int d;
	int64_t n = (int64_t) num;

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
	register const char *str;

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
						if ((int64_t) num < 0) {
							putch('-');
							num = (int64_t) num * -1;
						}
						base = 10;
						printnum(putch, num, base);
						break;

					case 'x':
						num = getuint(va);
						base = 16;
						printnum(putch, num, base);
						break;

					case 's':
						str = va_arg(*va, char *);
						if (!str)
							str = "(null)";
						while (*str != '\0') 
							putch(*str++);
				}

				break;

			default:
				putch(ch);
				break;
		}

	}
}

