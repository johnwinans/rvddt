#include "memcard.h"

#include <stdint.h>

int32_t memcard::get32(uint64_t addr)
{
	if (addr == statusRegAddr)
		return status;
	else if (addr == controlRegAddr)
		return control;
	else if (addr == addressRegAddr)
		return address;

	return buf[(addr-bufFirstAddr)/4];
}

void memcard::set32(uint64_t addr, uint32_t val)
{
	if (addr == statusRegAddr)
		return;							// ignore a write to the status register
	else if (addr == controlRegAddr)
		writeControl(val);
	else if (addr == addressRegAddr)
		writeAddress(val);
	else
		writeBuf(addr, val);
}

void memcard::writeControl(uint32_t val)
{
	control = val;
	if (control == controlWriteCommand)
	{
		printf("%s: Begin write operation...\n", getIdent());
	}
	else if (control == controlReadCommand)
	{
		printf("%s: Begin read operation...\n", getIdent());
	}
	else
	{
		devices::errorInvalidWrite("32", controlRegAddr, val, "Invalid command value");
	}
}

void memcard::writeAddress(uint32_t val)
{
	address = val;
}

void memcard::writeBuf(uint64_t addr, uint32_t val)
{
	buf[(addr-bufFirstAddr)/4] = val;
}

#if 0
void memcard::dump()
{
}
#endif
