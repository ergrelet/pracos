#ifndef MISC_H
#define MISC_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);

void *memset(void *s, int32_t c, size_t n);

int islower(int c);
int toupper(int c);


#endif
