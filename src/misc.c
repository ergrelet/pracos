#include <misc.h>

size_t
strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

void*
memset(void* s, int32_t c, size_t n) {
	uint8_t* p = (uint8_t*)s;
	while(n--) {
		*p++ = (uint8_t)c;
	}
	
	return s;
}
