#ifndef __BACKEND__
#define __BACKEND__

#include <chrono>

#include "Display.hpp"
#include "Keyboard.hpp"

namespace Chip8
{
	class Backend
	{
		public:
			virtual void display(const Display& display) = 0;
			virtual void clearDisplay() = 0;
			virtual void playBeep() = 0;

			virtual bool keyIsPressed(Key key) = 0;
			virtual Key waitKey() = 0;
			virtual bool quit() = 0;

			virtual std::chrono::milliseconds getTicks() = 0;
			virtual void delay(const std::chrono::milliseconds& time) = 0;
	};
}

#endif
