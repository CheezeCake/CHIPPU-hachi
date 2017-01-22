#ifndef __CLOCK_HPP__
#define __CLOCK_HPP__

#include <chrono>
#include <sys/time.h>

namespace Chip8
{
	class Clock
	{
		public:
			Clock();
			std::chrono::milliseconds getTicks() const;

		private:
			timespec mStart;
	};
}

#endif
