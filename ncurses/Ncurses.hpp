#ifndef __NCURSES_HPP__
#define __NCURSES_HPP__

#include <map>

#include <CHIP-8/Backend.hpp>
#include "Clock.hpp"

namespace Chip8
{
	class Ncurses : public Chip8::Backend
	{
		public:
			Ncurses();
			~Ncurses();

			virtual bool keyIsPressed(Chip8::Key key) override;
			virtual Chip8::Key waitKey() override;
			virtual bool quit() override;

			virtual void display(const Chip8::Display& display) override;
			virtual void clearDisplay() override;
			virtual void playBeep() override;
			virtual void stopBeep() override;

			virtual std::chrono::milliseconds getTicks() override;
			virtual void delay(const std::chrono::milliseconds& time) override;

		private:
			const std::map<Key, int> Chip8KeyToNcurses;
			const std::map<int, Key> NcursesToChip8Key;

			bool mPlayBeep = false;
			Chip8::Clock mClock;
	};
}

#endif
