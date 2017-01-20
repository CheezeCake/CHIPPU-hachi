#include <iostream>
#include <unistd.h>

#include <CHIP-8/VM.hpp>
#include "SDL2.hpp"

int main(int argc, char** argv)
{
	SDL2 sys;
	Chip8::VM vm(sys, std::cin);

	vm.run();

	return 0;
}
