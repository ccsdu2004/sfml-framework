#pragma once
#include <cstdint>

class Bitmask
{
public:
    Bitmask(uint32_t value = 0);
public:
    void setMask(const Bitmask& other);
    uint32_t getMask() const;
    bool contain(int pos) const;
    void setBit(int pos, bool on);
    void setBit(int pos);
    void clearBit(int pos);
    void clear();
private:
    uint32_t bits;
};
