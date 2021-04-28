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

#ifndef memory_H
#define memory_H

#include <stdint.h>
#include <string>

constexpr uint32_t devBaseAddress = 0xf0000000;

class memory
{
public:
	memory(uint64_t start, uint64_t length);
	~memory();

	int8_t get8(uint64_t addr);
	int16_t get16(uint64_t addr);
	int32_t get32(uint64_t addr);
	int64_t get64(uint64_t addr);

    void set8(uint64_t addr, uint8_t val);
    void set16(uint64_t addr, uint16_t val);
    void set32(uint64_t addr, uint32_t val);
    void set64(uint64_t addr, uint64_t val);

	void dump(uint64_t addr, uint64_t length);

	void readRaw(const char *filename, uint64_t addr);

	void setMemoryWarnings(int i) { memoryWarnings=i; }
	int getMemoryWarnings() const { return memoryWarnings; } 

private:
	int8_t *mem;		// the actual memory buffer
	uint64_t start;
	uint64_t len;
	int memoryWarnings;
};

extern memory *mem;


#endif
