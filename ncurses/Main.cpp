#include <iostream>
#include <csignal>

#include <CHIP-8/VM.hpp>

#include "Ncurses.hpp"

int main(int argc, char** argv)
{
	Chip8::Ncurses ncurses;
	Chip8::VM vm(ncurses);

	if (argc > 1)
		vm.loadProgram(argv[1]);
	else
		vm.loadProgram(std::cin);

	vm.run();

	return 0;
}
