#ifndef __DISPLAY__
#define __DISPLAY__

#include <array>

namespace Chip8
{
	class Display
	{
		public:
			static const unsigned int WIDTH = 64;
			static const unsigned int HEIGHT = 32;

			static const unsigned int SPRITE_WIDTH = 8;
			static const unsigned int SPRITE_HEIGHT = 15;

			void clear();
			bool getPixel(unsigned int x, unsigned int y) const;
			// returns true if the pixel was flipped from set to unset, false otherwise
			bool flipPixel(unsigned int x, unsigned int y);

			void dump() const;

		private:
			std::array<std::array<bool, WIDTH>, HEIGHT> mDisplay;
	};
}

#endif
