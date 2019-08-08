#ifndef devices_H
#define devices_H

#include <stdint.h>
#include <set>

//#include "dev/device.h"
class device;

class devices
{
public:
	devices();
	~devices();

	int8_t get8(uint64_t addr);
	int16_t get16(uint64_t addr);
	int32_t get32(uint64_t addr);
	int64_t get64(uint64_t addr);

    void set8(uint64_t addr, uint8_t val);
    void set16(uint64_t addr, uint16_t val);
    void set32(uint64_t addr, uint32_t val);
    void set64(uint64_t addr, uint64_t val);

	void dump();

	void setWarnings(int i) { warnings=i; }
	int getWarnings() const { return warnings; } 

	void addDevice(device *d) { devs.insert(d); }

	static void errorGet(const char *len, uint64_t addr);
	static void errorSet(const char *len, uint64_t addr, uint64_t val);


private:
	int warnings;

	std::set<device *>	devs;
};

#endif
