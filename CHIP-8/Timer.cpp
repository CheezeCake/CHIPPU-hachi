#include <iostream>

#include "Timer.hpp"

using namespace Chip8;

const std::chrono::milliseconds Timer::resolution(1000 / 60); // 60Hz

Timer::Timer() : Timer(0)
{
}

Timer::Timer(uint16_t value) :
	mValue(value)
{
}

void Timer::update(const std::chrono::milliseconds& delta)
{
	if (mValue == 0)
		return;

	const auto n = delta / resolution;

	if (n > mValue)
		mValue = 0;
	else
		mValue -= n;

	if (mCallback && mValue != 0)
		mCallback();
}

uint8_t Timer::getValue()
{
	return mValue;
}

void Timer::setValue(uint8_t value)
{
	mValue = value;
}

void Timer::setNonZeroCallback(const std::function<void()>& callback)
{
	mCallback = callback;
}
