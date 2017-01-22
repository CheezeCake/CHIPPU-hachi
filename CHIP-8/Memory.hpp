#ifndef __MEMORY__
#define __MEMORY__

#include <array>
#include <cstdint>

namespace Chip8
{
	class Memory
	{
		public:
			using addr_t = uint16_t;

			static const unsigned int MEMORY_SIZE = 4096;
			static const unsigned int CHARACTER_SPRITE_HEIGHT = 5;
			static const addr_t PROGRAM_START_ADDRESS = 512;

			constexpr addr_t getCharacterSpriteAddress(uint8_t c)
			{
				return (c * CHARACTER_SPRITE_HEIGHT);
			}

			void write(const char* buffer, std::size_t size, addr_t address);
			void write(uint8_t v, addr_t address);

			uint8_t readUint8(addr_t address);
			uint16_t readUint16(addr_t address);

			uint16_t readOpCode(addr_t address);

		private:
			std::array<uint8_t, MEMORY_SIZE> mRam = {
				{
				// 0
				0b11110000,
				0b10010000,
				0b10010000,
				0b10010000,
				0b11110000,
				// 1
				0b00100000,
				0b01100000,
				0b00100000,
				0b00100000,
				0b01110000,
				// 2
				0b11110000,
				0b00010000,
				0b11110000,
				0b10000000,
				0b11110000,
				// 3
				0b11110000,
				0b00010000,
				0b11110000,
				0b00010000,
				0b11110000,
				// 4
				0b10010000,
				0b10010000,
				0b11110000,
				0b00010000,
				0b00010000,
				// 5
				0b11110000,
				0b10000000,
				0b11110000,
				0b00010000,
				0b11110000,
				// 6
				0b11110000,
				0b10000000,
				0b11110000,
				0b10010000,
				0b11110000,
				// 7
				0b11110000,
				0b00010000,
				0b00100000,
				0b01000000,
				0b01000000,
				// 8
				0b11110000,
				0b10010000,
				0b11110000,
				0b10010000,
				0b11110000,
				// 9
				0b11110000,
				0b10010000,
				0b11110000,
				0b00010000,
				0b11110000,
				// A
				0b11110000,
				0b10010000,
				0b11110000,
				0b10010000,
				0b10010000,
				// B
				0b11100000,
				0b10010000,
				0b11100000,
				0b10010000,
				0b11100000,
				// C
				0b11110000,
				0b10000000,
				0b10000000,
				0b10000000,
				0b11110000,
				// D
				0b11100000,
				0b10010000,
				0b10010000,
				0b10010000,
				0b11100000,
				// E
				0b11110000,
				0b10000000,
				0b11110000,
				0b10000000,
				0b11110000,
				// F
				0b11110000,
				0b10000000,
				0b11110000,
				0b10000000,
				0b10000000
				}
			};
	};
}

#endif
