#include <iostream>
#include <cstdlib>

#include <CHIP-8/VM.hpp>
#include "SDL2.hpp"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " game\n";
		return 0;
	}

	Chip8::SDL2 sys;
	Chip8::VM vm(sys, argv[1]);

	vm.run();

	exit(0);
}
