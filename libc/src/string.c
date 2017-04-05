#include "../string.h"

int strlen(const char *str)
{
	int len;
	while(str[len])
		++len;
	return len;
}

char *strncpy(char *dest, const char *src, unsigned int len)
{
	register unsigned int i;
	
	for(i = 0; i < len && src[i] != '\0'; ++i)
		dest[i] = src[i];
	for(; i < len; ++i)
		dest[i] = '\0';

	return dest;
}
