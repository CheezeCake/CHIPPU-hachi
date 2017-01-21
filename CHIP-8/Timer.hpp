#ifndef __TIMER__
#define __TIMER__

#include <chrono>
#include <cstdint>
#include <functional>

namespace Chip8
{
	class Timer
	{
		public:
			Timer();
			Timer(uint16_t value);

			void update(const std::chrono::milliseconds& delta);
			uint8_t getValue();
			void setValue(uint8_t value);
			void setNonZeroCallback(const std::function<void()>& callback);
			void setZeroCallback(const std::function<void()>& callback);

			static const std::chrono::milliseconds resolution;

		private:
			uint8_t mValue;
			std::function<void()> mNonZeroCallback;
			std::function<void()> mZeroCallback;
	};
}

#endif
