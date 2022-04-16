#include <Bitmask.h>

Bitmask::Bitmask(uint32_t value):
    bits(value)
{
}

void Bitmask::setMask(const Bitmask &other)
{
    bits = other.getMask();
}

uint32_t Bitmask::getMask() const
{
    return bits;
}

bool Bitmask::contain(int pos) const
{
    return (bits & (1 << pos)) != 0;
}

void Bitmask::setBit(int pos, bool on)
{
    if (on)
        setBit(pos);
    else
        clearBit(pos);
}

void Bitmask::setBit(int pos)
{
    bits = bits | 1 << pos;
}

void Bitmask::clearBit(int pos)
{
    bits = bits & ~(1 << pos);
}

void Bitmask::clear()
{
    bits = 0;
}
