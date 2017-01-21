#include <iostream>

#include "Display.hpp"

using namespace Chip8;

void Display::clear()
{
	for (auto& line : mDisplay)
		for (auto& column : line)
			column = 0;
}

bool Display::getPixel(unsigned int x, unsigned int y) const
{
	return mDisplay[y][x];
}

bool Display::xorPixel(unsigned int x, unsigned int y, bool newPixel)
{
	const bool oldPixel = mDisplay[y][x];
	mDisplay[y][x] = oldPixel ^ newPixel;

	return (oldPixel && !mDisplay[y][x]);
}

void Display::dump() const
{
	for (const auto& line : mDisplay) {
		for (const auto& column : line)
			std::cout << ((column) ? '#' : ' ');
		std::cout << '\n';
	}
}
