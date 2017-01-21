#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "VM.hpp"
#include "Keyboard.hpp"

using namespace Chip8;

const std::chrono::milliseconds VM::runningRate(1000 / 840); // 840Hz

VM::VM(Backend& backend, std::istream& stream) :
	mBackend(backend)
{
	loadProgram(stream);
	init();
}

VM::VM(Backend& backend, const std::string& filename) :
	mBackend(backend)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file)
		throw std::runtime_error("error opening file: " + filename);

	loadProgram(file);
	init();
}

void VM::init()
{
	srand(time(nullptr));

	mSoundTimer.setNonZeroCallback([&] { mBackend.playBeep(); });
	mSoundTimer.setZeroCallback([&] { mBackend.stopBeep(); });
	mDisplay.clear();
}

void VM::previousInst()
{
	mRegisters.pc -= OPCODE_SIZE;
}

void VM::nextInst()
{
	mRegisters.pc += OPCODE_SIZE;
}

void VM::loadProgram(std::istream& stream)
{
	constexpr int BUFFER_SIZE = 512;

	char buffer[BUFFER_SIZE];
	Memory::addr_t address = Memory::PROGRAM_START_ADDRESS;

	while (!stream.eof()) {
		stream.read(buffer, BUFFER_SIZE);
		const auto count = stream.gcount();
		mMemory.write(buffer, count, address);
		address += count;
	}
}

void VM::run()
{
	auto runningTime = mBackend.getTicks();
	auto timersTime = runningTime;

	mRegisters.pc = Memory::PROGRAM_START_ADDRESS;

	while (!mBackend.quit()) {
		const auto current = mBackend.getTicks();

		auto delta = current - runningRate;
		if (delta >= runningRate) {
			runExec();
			runningTime = current;
		}

		delta = current - timersTime;
		if (delta >= Timer::resolution) {
			mDelayTimer.update(delta);
			mSoundTimer.update(delta);
			timersTime = current;
		}

		mBackend.delay(std::chrono::milliseconds(runningRate));
	}
}

void VM::runExec()
{
	uint16_t opCode = mMemory.readOpCode(mRegisters.pc);
	/* std::cout << '[' << std::hex << mRegisters.pc << "] opcode: 0x" << std::hex << opCode << " (" << std::dec << opCode << ")\n"; */
	nextInst();

	const auto opCodeTuple = std::make_tuple(
		opCode >> 12,
		(opCode >> 8) & 0xf,
		(opCode >> 4) & 0xf,
		opCode & 0xf
	);

	const auto x = std::get<1>(opCodeTuple);
	const auto y = std::get<2>(opCodeTuple);

	switch (std::get<0>(opCodeTuple)) {
		case 0:
			if (opCode == 0x00e0) {
				mDisplay.clear();
				mBackend.clearDisplay();
			}
			else if (opCode == 0x00ee) {
				execReturn();
			}
			// ignore call to RCA 1802 program
			break;
		case 1:
			// jump
			mRegisters.pc = opCode & 0xfff;
			break;
		case 2:
			// call
			mStack.push(mRegisters.pc);
			mRegisters.pc = opCode & 0xfff;
			break;
		case 3:
			if (mRegisters.V[x] == (opCode & 0xff))
				nextInst();
			break;
		case 4:
			if (mRegisters.V[x] != (opCode & 0xff))
				nextInst();
			break;
		case 5:
			if (std::get<3>(opCodeTuple) != 0)
				throw InvalidInstruction(opCode);
			if (mRegisters.V[x] == mRegisters.V[y])
				nextInst();
			break;
		case 6:
			mRegisters.V[x] = opCode & 0xff;
			break;
		case 7:
			mRegisters.V[x] += opCode & 0xff;
			break;
		case 8:
			handle8xyOps(opCode, x, y, std::get<3>(opCodeTuple));
			break;
		case 9:
			if (std::get<3>(opCodeTuple) != 0)
				throw InvalidInstruction(opCode);
			if (mRegisters.V[x] != mRegisters.V[y])
				nextInst();
			break;
		case 0xa:
			mRegisters.I = opCode & 0xfff;
			break;
		case 0xb:
			mRegisters.pc = mRegisters.V[0] + (opCode & 0xfff);
			break;
		case 0xc:
			mRegisters.V[x] = (rand() % 256) & (opCode & 0xff);
			break;
		case 0xd:
			mRegisters.V[0xf] = execDraw(mRegisters.V[x], mRegisters.V[y], opCode & 0xf);
			break;
		case 0xe:
			execKeyCheck(x, opCode);
			break;
		case 0xf:
			handleFxOps(opCode, x);
			break;
		default:
			throw InvalidInstruction(opCode);
	}
}

void VM::handle8xyOps(uint16_t opCode, uint8_t x, uint8_t y, uint8_t LSHB)
{
	auto& Vx = mRegisters.V[x];
	auto& Vy = mRegisters.V[y];
	auto& Vf = mRegisters.V[0xf];

	switch (LSHB) {
		case 0:
			Vx = Vy;
			break;
		case 1:
			Vx |= Vy;
			break;
		case 2:
			Vx &= Vy;
			break;
		case 3:
			Vx ^= Vy;
			break;
		case 4:
			Vx += Vy;
			// carry
			Vf = (Vx < Vy);
			break;
		case 5:
			// not borrow
			Vf = (Vx > Vy);
			Vx -= Vy;
			break;
		case 6:
			Vf = Vx & 1;
			Vx >>= 1;
			break;
		case 7:
			// not borrow
			Vf = (Vy > Vx);
			Vx = Vy - Vx;
			break;
		case 0xe:
			Vf = Vx >> 7;
			Vx <<= 1;
			break;
		default:
			throw InvalidInstruction(opCode);
	}
}

void VM::handleFxOps(uint16_t opCode, uint8_t x)
{
	switch (opCode & 0xff) {
		case 0x07:
			mRegisters.V[x] = mDelayTimer.getValue();
			break;
		case 0x0a:
			execGetKey(x);
			break;
		case 0x15:
			mDelayTimer.setValue(mRegisters.V[x]);
			break;
		case 0x18:
			mSoundTimer.setValue(mRegisters.V[x]);
		case 0x1e:
			mRegisters.I += mRegisters.V[x];
			mRegisters.V[0xf] = (mRegisters.I > 0xfff);
			break;
		case 0x29:
			mRegisters.I = mMemory.getCharacterSpriteAddress(mRegisters.V[x]);
			break;
		case 0x33:
			mMemory.write(static_cast<uint8_t>((mRegisters.V[x] / 100) % 10), mRegisters.I);
			mMemory.write(static_cast<uint8_t>((mRegisters.V[x] / 10) % 10), mRegisters.I + 1);
			mMemory.write(static_cast<uint8_t>(mRegisters.V[x] % 10), mRegisters.I + 2);
			break;
		case 0x55:
			execRegDump(x);
			break;
		case 0x65:
			execRegLoad(x);
			break;
		default:
			throw InvalidInstruction(opCode);
	}
}

void VM::execRegDump(uint8_t x)
{
	for (int i = 0; i <= x; ++i)
		mMemory.write(mRegisters.V[i], mRegisters.I + i);
}

void VM::execRegLoad(uint8_t x)
{
	for (int i = 0; i <= x; ++i)
		mRegisters.V[i] = mMemory.readUint8(mRegisters.I + i);
}

void VM::execReturn()
{
	mRegisters.pc = mStack.top();
	mStack.pop();
}

void VM::execGetKey(uint8_t x)
{
	Key key = mBackend.waitKey();

	if (key == Key::KEY_NONE)
		previousInst();
	else
		mRegisters.V[x] = key;
}

void VM::execKeyCheck(uint8_t x, uint16_t opCode)
{
	const bool pressed = mBackend.keyIsPressed(static_cast<Key>(mRegisters.V[x]));
	bool expected;

	switch (opCode & 0xff) {
		case 0x9e:
			expected = true;
			break;
		case 0xa1:
			expected = false;
			break;
		default:
			throw InvalidInstruction(opCode);
	}

	if (pressed == expected)
		nextInst();
}

bool VM::execDraw(uint8_t x, uint8_t y, uint8_t n)
{
	bool collision = false;

	for (int i = 0; i < n; ++i) {
		uint8_t sprite = mMemory.readUint8(mRegisters.I + i);
		/* const uint8_t y_ = (y + i) % Display::HEIGHT; */
		const uint8_t y_ = (y + i);

		if (y_ < Display::HEIGHT) {
			for (unsigned int j = 0; j < Display::SPRITE_WIDTH; ++j) {
				/* const uint8_t x_ = (x + j) % Display::WIDTH; */
				const uint8_t x_ = (x + j);

				if (x_ < Display::WIDTH) {
					const bool pixel = (sprite >> 7) & 1;

					if (pixel && mDisplay.flipPixel(x_, y_))
						collision = true;

					sprite <<= 1;
				}
			}
		}
	}

	mBackend.display(mDisplay);

	return collision;
}


/*
 * InvalidInstruction exception
 */
VM::InvalidInstruction::InvalidInstruction(uint16_t opCode) :
	std::runtime_error("Invalid OpCode: " + opCodeToHexStr(opCode))
{
}

std::string VM::InvalidInstruction::opCodeToHexStr(uint16_t opCode)
{
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(opCode) * 2)
		<< std::hex << opCode;

	return stream.str();
}
