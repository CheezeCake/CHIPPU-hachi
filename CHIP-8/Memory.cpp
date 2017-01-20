#include "Memory.hpp"
#include "Display.hpp"
#include <iostream>

using namespace Chip8;

void Memory::write(const char* buffer, std::size_t size, addr_t address)
{
	for (std::size_t i = 0; i < size; ++i)
		mRam[address + i] = buffer[i];
}

void Memory::write(uint8_t v, addr_t address)
{
	mRam[address] = v;
}

uint8_t Memory::readUint8(addr_t address)
{
	return mRam[address];
}

uint16_t Memory::readUint16(addr_t address)
{
	return (mRam[address] << 8 | mRam[address + 1]);
}

uint16_t Memory::readOpCode(addr_t address)
{
	return ((mRam[address] << 8) | mRam[address + 1]);
}
