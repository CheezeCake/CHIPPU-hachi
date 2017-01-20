#ifndef __FRONTEND__
#define __FRONTEND__

#include "Keyboard.hpp"

namespace Chip8
{
	class Frontend
	{
		public:
			virtual bool keyIsPressed(Key key) = 0;
			virtual Key waitKey() = 0;
			virtual bool quit() = 0;
	};
}

#endif
