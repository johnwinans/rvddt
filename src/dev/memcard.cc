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
