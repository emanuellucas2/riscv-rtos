#ifndef _LIBC_H_
#define _LIBC_H_

#include <types.h>
#include <stdarg.h>

#define min(a,b)		((a)<(b)?(a):(b))
#define max(a,b)        ((a)>(b)?(a):(b))

void *memset(void *dst, int c, unsigned long bytes);
void *memcpy(void *dst, const void *src, unsigned long bytes);
int32_t puts(const char *str);
char *itoa(int i, char *s, int base);
int32_t printf(const char *format, ...);
int32_t sprintf(char *out, const char *format, ...);
int32_t strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
uint32_t strlen(const char *str);
uint32_t hash(unsigned char *str); 


#endif
