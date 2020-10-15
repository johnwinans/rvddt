#ifndef console_H
#define console_H

#include "device.h"

#include <stdint.h>

class console : public device
{
public:
    void set8(uint64_t addr, uint8_t val) { putchar(val); }

    uint64_t getBaseAddress() { return 0xffff0001; }
    uint64_t getLastAddress() { return 0xffff0001; }
    const char *getIdent() { return "CON"; }

private:
};

#endif
