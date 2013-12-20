#include <string.h>

void memcpy(void *dst, const void *src, size_t n)
{
	char *d = (char *) dst;
	char *s = (char *) src;

	while (n--) {
		*d++ = *s++;
	}
}

void memset(void *dst, int c, size_t n)
{
	char *d = (char *) dst;

	while (n--) {
		*d++ = c;
	}
}

int strlen(const char *s)
{
	int n = 0;
	char *c = (char *) s;

	while (*c++ != '\0') 
		n++;

	return n+1;
}
