#include <types.h>
#include <printk.h>


void cwshell()
{
	char c;

	while (1) {
		getchar(&c);
		if (c == '\0')
			continue;
		putchar(c);
	}
}
