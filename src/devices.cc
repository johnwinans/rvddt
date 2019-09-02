#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "devices.h"
#include "dev/device.h"
#include "main.h"

devices::devices()
{
	setWarnings(1);
}

devices::~devices()
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
		delete *itr;
}

int8_t devices::get8(uint64_t addr)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
			return pdev->get8(addr);
	}
	errorGet("8", addr);
	return -1;		// invalid devices area, act like a hi-z TTL bus
}

int16_t devices::get16(uint64_t addr)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
			return pdev->get16(addr);
	}
	errorGet("16", addr);
	return -1;		// invalid devices area, act like a hi-z TTL bus
}

int32_t devices::get32(uint64_t addr)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
			return pdev->get32(addr);
	}
	errorGet("32", addr);
	return -1;		// invalid devices area, act like a hi-z TTL bus
}

int64_t devices::get64(uint64_t addr)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
			return pdev->get64(addr);
	}
	errorGet("64", addr);
	return -1;		// invalid devices area, act like a hi-z TTL bus
}

void devices::set8(uint64_t addr, uint8_t val)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
		{ 
			pdev->set8(addr, val);
			return;
		}
	}
	errorSet("8", addr, val);
}

void devices::set16(uint64_t addr, uint16_t val)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
		{ 
			pdev->set16(addr, val);
			return;
		}
	}
	errorSet("16", addr, val);
}

void devices::set32(uint64_t addr, uint32_t val)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
		{ 
			pdev->set32(addr, val);
			return;
		}
	}
	errorSet("32", addr, val);
}

void devices::set64(uint64_t addr, uint64_t val)
{
	for (auto itr = devs.begin(); itr != devs.end(); ++itr)
	{
		device *pdev = *itr;
		if (pdev->getBaseAddress() <= addr && pdev->getLastAddress() >= addr)
		{ 
			pdev->set64(addr, val);
			return;
		}
	}
	errorSet("64", addr, val);
}


void devices::errorSet(const char *len, uint64_t addr, uint64_t val)
{
	printf("WARNING: %s-bit write to non-existent device at address: 0x%8.8x = 0x%8.8x\n", len, (uint32_t)addr, (uint32_t)val);
}

void devices::errorGet(const char *len, uint64_t addr)
{
	printf("WARNING: %s-bit read from non-existent device at address: 0x%8.8x\n", len, (uint32_t)addr);
}

void devices::errorInvalidWrite(const char *len, uint64_t addr, uint64_t val, const char *msg)
{
	printf("WARNING: %s-bit write to address 0x%8.8x with illegal value 0x%8.8x\n", len, (uint32_t)addr, (uint32_t)val);

	if (msg)
		printf("%s\n", msg);
}
