#ifndef memory_H
#define memory_H

#include <stdint.h>
#include <string>

class memory
{
public:
	memory(uint64_t start, uint64_t length);
	~memory();
	
	/*
	 * These functions fetch memory from a given address in memory.
	 */

	//Fetches one byte from the given address in memory.
	int8_t get8(uint64_t addr) const;
	//Fetches two bytes from the given address in memory.
	int16_t get16(uint64_t addr) const;
	//Fetches four bytes from the given address in memory.
	int32_t get32(uint64_t addr) const;
	//Fetches eight bytes from the given address in memory.
	int64_t get64(uint64_t addr) const;

	/*
	 * These functions set memory at a given address at a given byte length
	 */

	//Sets one byte of  memory at the given address.
    	void set8(uint64_t addr, uint8_t val);
	//Sets two bytes of memory at the given address.
    	void set16(uint64_t addr, uint16_t val);
	//Sets four bytes of memory at the given address.
    	void set32(uint64_t addr, uint32_t val);
	//Sets eight bytes of memory at the given address
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
