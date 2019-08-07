#ifndef devices_H
#define devices_H

#include <stdint.h>

class devices
{
public:
	devices(const char *sdFilename);
	~devices();

#if 1
	int8_t get8(uint64_t addr) { return 0; }
	int16_t get16(uint64_t addr) { return 0; }
	int32_t get32(uint64_t addr) { return 0; }
	int64_t get64(uint64_t addr) { return 0; }

    void set8(uint64_t addr, uint8_t val) {}
    void set16(uint64_t addr, uint16_t val) {}
    void set32(uint64_t addr, uint32_t val) {}
    void set64(uint64_t addr, uint64_t val) {}
#else
	int8_t get8(uint64_t addr);
	int16_t get16(uint64_t addr);
	int32_t get32(uint64_t addr);
	int64_t get64(uint64_t addr);

    void set8(uint64_t addr, uint8_t val);
    void set16(uint64_t addr, uint16_t val);
    void set32(uint64_t addr, uint32_t val);
    void set64(uint64_t addr, uint64_t val);

	void dump();
#endif

	void setWarnings(int i) { warnings=i; }
	int getWarnings() const { return warnings; } 

private:
	int warnings;
};

#endif
