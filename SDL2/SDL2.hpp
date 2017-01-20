#ifndef __SDL2_HPP__
#define __SDL2_HPP__

#include <map>

#include <SDL2/SDL.h>

#include <CHIP-8/Backend.hpp>

class SDL2 : public Chip8::Backend
{
	public:
		SDL2();
		~SDL2();

		virtual bool keyIsPressed(Chip8::Key key) override;
		virtual Chip8::Key waitKey() override;
		virtual bool quit() override;

		virtual void display(const Chip8::Display& display) override;
		virtual void clearDisplay() override;
		virtual void playBeep() override;

		virtual std::chrono::milliseconds getTicks() override;
		virtual void delay(const std::chrono::milliseconds& time) override;

		static int eventFilter(void* userdata, SDL_Event* event);

		static const unsigned int PIXELS_PER_CHIP8_PIXEL = 10;
		static const unsigned int SCREEN_WIDTH =
			Chip8::Display::WIDTH * PIXELS_PER_CHIP8_PIXEL;
		static const unsigned int SCREEN_HEIGHT =
			Chip8::Display::HEIGHT * PIXELS_PER_CHIP8_PIXEL;

	private:
		void colorPixel(Uint8* pixel, Uint32 color, Uint8 bpp);

		SDL_Window* mWindow = nullptr;
		SDL_Surface* mWindowSurface = nullptr;

		Uint32 mBlack;
		Uint32 mWhite;

		static const std::map<Chip8::Key, int> Chip8KeyToSDL;
		static const std::map<Chip8::Key, int> Chip8KeyToSDLKeypad;
		static const std::map<int, Chip8::Key> SDLToChip8Key;
};

#endif
