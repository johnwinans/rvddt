#include <stdint.h>

uint32_t d1 = 0xdddddddd;
uint32_t d2[1000] = { 1,2,3,4,5,6,7,8,9,10 };
uint32_t d3[1000] = { 10,9,8,7,6,5,4,3,2,1 };

uint32_t b1;
uint32_t b2[4];

int main()
{
	b2[3] = 0x8675309;

	d3[0] = 0x12345678;
	d2[0] = 0x87654321;

	return 0;
}
