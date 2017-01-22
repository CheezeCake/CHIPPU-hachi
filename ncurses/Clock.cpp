#include <ctime>
#include <cstdint>

#include "Clock.hpp"

using namespace Chip8;

Clock::Clock()
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &mStart);
}

std::chrono::milliseconds Clock::getTicks() const
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);

	return std::chrono::milliseconds((now.tv_sec - mStart.tv_sec) * 1000 +
			(now.tv_nsec - mStart.tv_nsec) / 1'000'000);
}
