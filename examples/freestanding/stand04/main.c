#include <stdint.h>

uint32_t b1;
uint32_t b2[4];

int main()
{
	b2[3] = 0x8675309;
	return 0;
}
