#include <iostream>
#include <unistd.h>

#include <curses.h>

#include "Ncurses.hpp"

using namespace Chip8;

const std::map<Key, int> Ncurses::Chip8KeyToNcurses = {
	{ Key::KEY_0, '0' },
	{ Key::KEY_1, '1' },
	{ Key::KEY_2, '2' },
	{ Key::KEY_3, '3' },
	{ Key::KEY_4, '4' },
	{ Key::KEY_5, '5' },
	{ Key::KEY_6, '6' },
	{ Key::KEY_7, '7' },
	{ Key::KEY_8, '8' },
	{ Key::KEY_9, '9' },
	{ Key::KEY_A, 'a' },
	{ Key::KEY_B, 'b' },
	{ Key::KEY_C, 'c' },
	{ Key::KEY_D, 'd' },
	{ Key::KEY_E, 'e' },
	{ Key::KEY_F, 'f' }
};

const std::map<int, Key> Ncurses::NcursesToChip8Key = {
	{ '0', Key::KEY_0 },
	{ '1', Key::KEY_1 },
	{ '2', Key::KEY_2 },
	{ '3', Key::KEY_3 },
	{ '4', Key::KEY_4 },
	{ '5', Key::KEY_5 },
	{ '6', Key::KEY_6 },
	{ '7', Key::KEY_7 },
	{ '8', Key::KEY_8 },
	{ '9', Key::KEY_9 },
	{ 'a', Key::KEY_A },
	{ 'b', Key::KEY_B },
	{ 'c', Key::KEY_C },
	{ 'd', Key::KEY_D },
	{ 'e', Key::KEY_E },
	{ 'f', Key::KEY_F }
};

Ncurses::Ncurses()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	attrset(A_REVERSE);
	mvaddch(10, 10, ' ');
}

Ncurses::~Ncurses()
{
	endwin();
}

bool Ncurses::keyIsPressed(Chip8::Key key)
{
	nodelay(stdscr, TRUE);
	const int ch = wgetch(stdscr);
	nodelay(stdscr, FALSE);

	return (ch != ERR && Chip8KeyToNcurses.count(key) != ch);
}

Key Ncurses::waitKey()
{
	while (true) {
		const int ch = wgetch(stdscr);
		if (NcursesToChip8Key.count(ch) != 0)
			return NcursesToChip8Key.at(ch);
	}
}

bool Ncurses::quit()
{
	if (mPlayBeep)
		beep();

	return false;
}

void Ncurses::display(const Display& display)
{
	for (auto i = 0; i < Display::WIDTH; ++i) {
		for (auto j = 0; j < Display::HEIGHT; ++j) {
			attrset((display.getPixel(i, j)) ? A_REVERSE : A_NORMAL);
			mvaddch(j, i, ' ');
		}
	}
}

void Ncurses::clearDisplay()
{
	clear();
}

void Ncurses::playBeep()
{
	mPlayBeep = true;
}

void Ncurses::stopBeep()
{
	mPlayBeep = false;
}

std::chrono::milliseconds Ncurses::getTicks()
{
	return mClock.getTicks();
}

void Ncurses::delay(const std::chrono::milliseconds& time)
{
	usleep(time.count() * 1000);
}
