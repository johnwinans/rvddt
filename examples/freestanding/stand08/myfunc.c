
#include "myfunc.h"	// always include my own decls so compiler will tell me if I mess up!

int myfunc(const char *s)
{
	int i = 0;

	// sum up all the character codes in the string
	while (*s)
		i += *s++;

	return i;
}
