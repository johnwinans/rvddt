#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "devices.h"
#include "main.h"

//XXX implement a simple 'drive' interface with a single-block DMA buffer
//XXX along with a control register recognizing read and write command opcodes 
//XXX and add a status register for delayed completion status
//XXX also include IRQ generation when that is ready in the CPU

devices::devices(const char *sdFilename)
{
//XXX open disc dev file
	setWarnings(1);
}

devices::~devices()
{
//XXX close the disc file
}

#if 0
int8_t devices::get8(uint64_t addr)
{
	if (addr < start || addr >= start+len)
	{
		if (getMemoryWarnings())
		{
			printf("WARNING: accessing non-existent device at address: 0x%8.8x", (uint32_t)addr);
#ifdef DEBUGGING_HACKS
			printf(", pc=0x%8.8x", cpu->getPc());
#endif
			printf("\n");
		}
		
		return 0xff;		// invalid devices area, act like a hi-z TTL bus
	}

	return mem[addr-start];
}

int16_t devices::get16(uint64_t addr)
{
	int16_t i = (uint8_t)get8(addr);
	i |= ((uint16_t)get8(addr+1))<<8;
	return i;
}

int32_t devices::get32(uint64_t addr)
{
	int32_t i = (uint16_t)get16(addr);
	i |= ((uint32_t)get16(addr+2))<<16;
	return i;
}

int64_t devices::get64(uint64_t addr)
{
	int64_t i = (uint32_t)get32(addr);
	i |= ((uint64_t)get32(addr+4))<<32;

	return i;
}

void devices::set8(uint64_t addr, uint8_t val)
{
#ifdef MAGIC_UART_TX_ADDRESS
	if (addr == MAGIC_UART_TX_ADDRESS)
	{
		putchar(val);
		return;
	}
#endif

	if (addr < start || addr >= start+len)
    {
		if (getMemoryWarnings())
		{
			printf("WARNING: accessing non-existent devices at address: 0x%8.8x", (uint32_t)addr);
#ifdef DEBUGGING_HACKS
			printf(", pc=0x%8.8x", cpu->getPc());
#endif
			printf("\n");
		}
        return;        // invalid devices area, throw it away
    }

	mem[addr-start] = val;
}

void devices::set16(uint64_t addr, uint16_t val)
{
	set8(addr, val&0x00ff);			// little-endian order
	set8(addr+1, (val>>8)&0x00ff);
}

void devices::set32(uint64_t addr, uint32_t val)
{
	set16(addr, val&0x0000ffff);			// little-endian order
	set16(addr+2, (val>>16)&0x0000ffff);
}

void devices::set64(uint64_t addr, uint64_t val)
{
	set32(addr, val&0x0000ffffffff);			// little-endian order
	set32(addr+4, (val>>32)&0x0000ffffffff);
}

#endif
