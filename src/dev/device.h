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

#ifndef device_H
#define device_H

#include "devices.h"

#include <stdint.h>
#include <string>


class device
{
public:
	device() {}
	virtual ~device() {}

	virtual int8_t get8(uint64_t addr) { devices::errorGet("8", addr); return -1; }
	virtual int16_t get16(uint64_t addr) { devices::errorGet("16", addr); return -1; }
	virtual int32_t get32(uint64_t addr) { devices::errorGet("32", addr); return -1; }
	virtual int64_t get64(uint64_t addr) { devices::errorGet("64", addr); return -1; }

    virtual void set8(uint64_t addr, uint8_t val) { devices::errorSet("8", addr, val); }
    virtual void set16(uint64_t addr, uint16_t val) { devices::errorSet("16", addr, val); }
    virtual void set32(uint64_t addr, uint32_t val) { devices::errorSet("32", addr, val); }
    virtual void set64(uint64_t addr, uint64_t val) { devices::errorSet("64", addr, val); }

	virtual void dump() {}

	virtual uint64_t getBaseAddress() = 0;
	virtual uint64_t getLastAddress() = 0;
	virtual const char *getIdent() = 0;

private:
};

#endif
