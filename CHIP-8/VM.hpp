#ifndef __CHIP_8__
#define __CHIP_8__

#include <string>
#include <stack>
#include <istream>
#include <atomic>
#include <array>
#include <stdexcept>
#include <iomanip>
#include <tuple>
#include <chrono>

#include "Memory.hpp"
#include "Timer.hpp"
#include "Display.hpp"
#include "Backend.hpp"

namespace Chip8
{
	class VM
	{
		public:
			static const unsigned int OPCODE_SIZE = 2;
			static const std::chrono::milliseconds runningRate;

			VM(Backend& backend, std::istream& stream);
			VM(Backend& backend, const std::string& filename);

			void run();

			class InvalidInstruction : public std::runtime_error
			{
				public:
					InvalidInstruction(uint16_t opCode);

					static std::string opCodeToHexStr(uint16_t opCode);
			};

		private:
			void init();

			void runExec();
			void loadProgram(std::istream& stream);
			void previousInst();
			void nextInst();
			void handle8xyOps(uint16_t opCode, uint8_t x, uint8_t y, uint8_t LSHB);
			void handleFxOps(uint16_t opCode, uint8_t x);

			void execReturn();
			bool execDraw(uint8_t x, uint8_t y, uint8_t n);
			void execKeyCheck(uint8_t x, uint16_t opCode);
			void execGetKey(uint8_t x);
			void execRegDump(uint8_t x);
			void execRegLoad(uint8_t x);

			Backend& mBackend;

			std::stack<Memory::addr_t> mStack;
			Memory mMemory;
			Timer mDelayTimer;
			Timer mSoundTimer;
			Display mDisplay;

			struct Registers
			{
				using register_t = uint8_t;
				uint16_t pc;
				uint16_t I;
				std::array<register_t, 16> V;
			} mRegisters;
	};
}

#endif
