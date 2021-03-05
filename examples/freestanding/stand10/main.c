#include "myfunc.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t b1;
uint32_t b2[4];

int main()
{
	char *s;

	s = malloc(0x100);
	strcpy(s, "Hello World");
	int rc = myfunc(s);
	free(s);

	return rc;
}
