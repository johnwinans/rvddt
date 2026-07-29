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

#ifndef console_H
#define console_H

#include "device.h"

#include <stdint.h>

class console : public device
{
public:
	console();
	~console();
    void set8(uint64_t addr, uint8_t val);
	int8_t get8(uint64_t addr);

    uint64_t getBaseAddress() { return 0xffff0000; }
    uint64_t getLastAddress() { return 0xffff0002; }
	constexpr static uint64_t statusRegAddr = { 0xffff0002 };
	constexpr static uint64_t TxRegAddr = { 0xffff0001 };
	constexpr static uint64_t RxRegAddr = { 0xffff0000 };
    const char *getIdent() { return "CON"; }

private:
	int8_t c;
	int s;
    struct termios *pOldt;
};

#endif
