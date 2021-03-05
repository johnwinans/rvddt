#include "myfunc.h"

#include <stdint.h>

uint32_t b1;
uint32_t b2[4];

int main()
{
	char s[100] = { 1,2,3 };

	return myfunc(s);
}
