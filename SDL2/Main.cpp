#include <iostream>
#include <cstdlib>

#include <CHIP-8/VM.hpp>
#include "SDL2.hpp"

int main(int argc, char** argv)
{
	Chip8::SDL2 sys;
	Chip8::VM vm(sys);

	if (argc > 1)
		vm.loadProgram(argv[1]);
	else
		vm.loadProgram(std::cin);

	vm.run();

	exit(0);
}
