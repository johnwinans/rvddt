#ifndef memcard_H
#define memcard_H

#include "device.h"

#include <stdint.h>
#include <string>

class memcard : public device
{
public:
	memcard() {}
	virtual ~memcard() {}

	int8_t get8(uint64_t addr) { devices::errorGet("8", addr); return -1; }
	int16_t get16(uint64_t addr) { devices::errorGet("16", addr); return -1; }
	int32_t get32(uint64_t addr);
	int64_t get64(uint64_t addr) { devices::errorGet("64", addr); return -1; }

    void set8(uint64_t addr, uint8_t val) { devices::errorSet("8", addr, val); }
    void set16(uint64_t addr, uint16_t val) { devices::errorSet("16", addr, val); }
    void set32(uint64_t addr, uint32_t val);
    void set64(uint64_t addr, uint64_t val) { devices::errorSet("64", addr, val); }

	void writeControl(uint32_t val);
	void writeAddress(uint32_t val);
	void writeBuf(uint64_t addr, uint32_t val);

	//virtual void dump();

	uint64_t getBaseAddress() { return 0xf0000000; }
	uint64_t getLastAddress() { return 0xf0000217; }
	const char *getIdent() { return "MEMCARD"; }

	constexpr static uint64_t statusRegAddr = { 0xf0000214 };
	constexpr static uint64_t controlRegAddr = { 0xf0000210 };
	constexpr static uint64_t addressRegAddr = { 0xf0000200 };
	constexpr static uint64_t bufFirstAddr = { 0xf0000000 };
	constexpr static uint64_t bufLastAddr = { 0xf00001ff };

	constexpr static uint32_t controlWriteCommand = { 0x80000001 };
	constexpr static uint32_t controlReadCommand = { 0x80000002 };

private:
	uint32_t buf[512/4] = { 0 };		// the device buffer
	uint32_t status = { 0 };		// the current status register value 
	uint32_t control = { 0 };		// the current control register value
	uint32_t address = { 0 };		// the current address register value
};

#endif
