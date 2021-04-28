//****************************************************************************
//
//    RVDDT (RISC-V Dynamic Debugging Tool)
//
//    Copyright (C) 2020  John Winans
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//    USA
//
//****************************************************************************

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>

#include "saferead.h"
#include "devices.h"
#include "memory.h"
#include "main.h"

memory *mem;

/**
****************************************************************************/
memory::memory(uint64_t start, uint64_t length)
{
	mem = new int8_t[length];
	memset(mem, 0xa5, length);		// fill with an EZ to recognize pattern
	this->start = start;
	len = length;
	setMemoryWarnings(1);
}

/**
****************************************************************************/
memory::~memory()
{
	delete mem;
	mem = 0;
}

/**
****************************************************************************/
int8_t memory::get8(uint64_t addr)
{
	if (addr >= devBaseAddress)
		return dev->get8(addr);

	if (addr < start || addr >= start+len)
	{
		if (getMemoryWarnings())
		{
			printf("WARNING: accessing non-existent memory at address: 0x%8.8x", (uint32_t)addr);
#ifdef DEBUGGING_HACKS
			printf(", pc=0x%8.8x", cpu->getPc());
#endif
			printf("\n");
		}
		
		return 0xff;		// invalid memory area, act like a hi-z TTL bus
	}

	return mem[addr-start];
}

/**
****************************************************************************/
int16_t memory::get16(uint64_t addr)
{
	if (addr >= devBaseAddress)
		return dev->get16(addr);

	int16_t i = (uint8_t)get8(addr);
	i |= ((uint16_t)get8(addr+1))<<8;
	return i;
}

/**
****************************************************************************/
int32_t memory::get32(uint64_t addr)
{
	if (addr >= devBaseAddress)
		return dev->get32(addr);

	int32_t i = (uint16_t)get16(addr);
	i |= ((uint32_t)get16(addr+2))<<16;
	return i;
}

/**
****************************************************************************/
int64_t memory::get64(uint64_t addr)
{
	if (addr >= devBaseAddress)
		return dev->get64(addr);

	int64_t i = (uint32_t)get32(addr);
	i |= ((uint64_t)get32(addr+4))<<32;

	return i;
}

/**
****************************************************************************/
void memory::set8(uint64_t addr, uint8_t val)
{
	if (addr >= devBaseAddress)
	{
		dev->set8(addr, val);
		return;
	}

	if (addr < start || addr >= start+len)
    {
		if (getMemoryWarnings())
		{
			printf("WARNING: accessing non-existent memory at address: 0x%8.8x", (uint32_t)addr);
#ifdef DEBUGGING_HACKS
			printf(", pc=0x%8.8x", cpu->getPc());
#endif
			printf("\n");
		}
        return;        // invalid memory area, throw it away
    }

	mem[addr-start] = val;
}

/**
****************************************************************************/
void memory::set16(uint64_t addr, uint16_t val)
{
	if (addr >= devBaseAddress)
	{
		dev->set16(addr, val);
		return;
	}

	set8(addr, val&0x00ff);			// little-endian order
	set8(addr+1, (val>>8)&0x00ff);
}

/**
****************************************************************************/
void memory::set32(uint64_t addr, uint32_t val)
{
	if (addr >= devBaseAddress)
	{
		dev->set32(addr, val);
		return;
	}

	set16(addr, val&0x0000ffff);			// little-endian order
	set16(addr+2, (val>>16)&0x0000ffff);
}

/**
****************************************************************************/
void memory::set64(uint64_t addr, uint64_t val)
{
	if (addr >= devBaseAddress)
	{
		dev->set32(addr, val);
		return;
	}

	set32(addr, val&0x0000ffffffff);			// little-endian order
	set32(addr+4, (val>>32)&0x0000ffffffff);
}

/**
* Read the given raw file into memory at the given address.
*****************************************************************/
void memory::readRaw(const char *filename, uint64_t addr)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Failed to open file '%s',  Reason: %s\n", filename, strerror(errno));
		return;
	}

	saferead(fd, (char*)(&mem[addr-start]), len-(addr-start));

	close(fd);
}

/**
****************************************************************************/
void memory::dump(uint64_t addr, uint64_t length)
{
	int				ch;
	int				i = 0;
	uint64_t		j;
	unsigned char	ascii[20];  /* to hold printable portion of string */

	if (length==0)
		return;

	int startingoffset = addr%16;
	if (startingoffset != 0)
	{
		// pre-padd the dump so it looks purdy
		fprintf(ddtout, " %8.8" PRIx64 ":%s", addr&~0x0f, startingoffset>8?" ":"");
		int i;
		for (i=0; i<startingoffset; ++i)
			fprintf(ddtout, "   ");
	}

	for(j=addr; j<(addr+length); ++j)
	{
		ch = get8(j);// ((char*)buff)[j];
		if ((j % 16) == 0)
		{
			if (j>addr)
			{
				ascii[i] = '\0';
				fprintf(ddtout, " %*s*%s*\n", startingoffset, "", ascii);
				startingoffset = 0;
			}
			fprintf(ddtout, " %8.8x:", (uint32_t)j);
			i = 0;
		}
		fprintf(ddtout, "%s%2.2x", (j%8==0&&j%16!=0)?"  ":" ", ch & 0xff);

		ascii[i] = ch;
		if ((ascii[i] >= 0x80)||(!isprint(ascii[i])))
			ascii[i] = '.';
		++i;
	}
	if (j%16 && j%16<9)
		fprintf(ddtout, " ");
	while (j%16)
	{
		fprintf(ddtout, "   ");
		++j;
	}
	ascii[i] = '\0';
	fprintf(ddtout, " %*s*%s*\n", startingoffset, "", ascii);
}
