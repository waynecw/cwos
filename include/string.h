#ifndef __STRING__
#define __STRING__

typedef unsigned int size_t;

void memcpy(void *dst, const void *src, size_t n);
void memset(void *dst, int c, size_t n);
int strlen(const char *s);


#endif
