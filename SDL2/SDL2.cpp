#include <iostream>
#include <stdexcept>

#include "SDL2.hpp"

using namespace Chip8;

const std::map<Key, int> SDL2::Chip8KeyToSDL = {
	{ Key::KEY_0, SDL_SCANCODE_0 },
	{ Key::KEY_1, SDL_SCANCODE_1 },
	{ Key::KEY_2, SDL_SCANCODE_2 },
	{ Key::KEY_3, SDL_SCANCODE_3 },
	{ Key::KEY_4, SDL_SCANCODE_4 },
	{ Key::KEY_5, SDL_SCANCODE_5 },
	{ Key::KEY_6, SDL_SCANCODE_6 },
	{ Key::KEY_7, SDL_SCANCODE_7 },
	{ Key::KEY_8, SDL_SCANCODE_8 },
	{ Key::KEY_9, SDL_SCANCODE_9 },
	{ Key::KEY_A, SDL_SCANCODE_A },
	{ Key::KEY_B, SDL_SCANCODE_B },
	{ Key::KEY_C, SDL_SCANCODE_C },
	{ Key::KEY_D, SDL_SCANCODE_D },
	{ Key::KEY_E, SDL_SCANCODE_E },
	{ Key::KEY_F, SDL_SCANCODE_F }
};

const std::map<Key, int> SDL2::Chip8KeyToSDLKeypad = {
	{ Key::KEY_0, SDL_SCANCODE_KP_0 },
	{ Key::KEY_1, SDL_SCANCODE_KP_1 },
	{ Key::KEY_2, SDL_SCANCODE_KP_2 },
	{ Key::KEY_3, SDL_SCANCODE_KP_3 },
	{ Key::KEY_4, SDL_SCANCODE_KP_4 },
	{ Key::KEY_5, SDL_SCANCODE_KP_5 },
	{ Key::KEY_6, SDL_SCANCODE_KP_6 },
	{ Key::KEY_7, SDL_SCANCODE_KP_7 },
	{ Key::KEY_8, SDL_SCANCODE_KP_8 },
	{ Key::KEY_9, SDL_SCANCODE_KP_9 },
};

const std::map<int, Key> SDL2::SDLToChip8Key= {
	{ SDL_SCANCODE_0, Key::KEY_0 },
	{ SDL_SCANCODE_1, Key::KEY_1 },
	{ SDL_SCANCODE_2, Key::KEY_2 },
	{ SDL_SCANCODE_3, Key::KEY_3 },
	{ SDL_SCANCODE_4, Key::KEY_4 },
	{ SDL_SCANCODE_5, Key::KEY_5 },
	{ SDL_SCANCODE_6, Key::KEY_6 },
	{ SDL_SCANCODE_7, Key::KEY_7 },
	{ SDL_SCANCODE_8, Key::KEY_8 },
	{ SDL_SCANCODE_9, Key::KEY_9 },
	{ SDL_SCANCODE_KP_0, Key::KEY_0 },
	{ SDL_SCANCODE_KP_1, Key::KEY_1 },
	{ SDL_SCANCODE_KP_2, Key::KEY_2 },
	{ SDL_SCANCODE_KP_3, Key::KEY_3 },
	{ SDL_SCANCODE_KP_4, Key::KEY_4 },
	{ SDL_SCANCODE_KP_5, Key::KEY_5 },
	{ SDL_SCANCODE_KP_6, Key::KEY_6 },
	{ SDL_SCANCODE_KP_7, Key::KEY_7 },
	{ SDL_SCANCODE_KP_8, Key::KEY_8 },
	{ SDL_SCANCODE_KP_9, Key::KEY_9 },
	{ SDL_SCANCODE_A, Key::KEY_A },
	{ SDL_SCANCODE_B, Key::KEY_B },
	{ SDL_SCANCODE_C, Key::KEY_C },
	{ SDL_SCANCODE_D, Key::KEY_D },
	{ SDL_SCANCODE_E, Key::KEY_E },
	{ SDL_SCANCODE_F, Key::KEY_F }
};

SDL2::SDL2()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(SDL_GetError());

	mWindow = SDL_CreateWindow("CHIPPU-hachi", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!mWindow)
		throw std::runtime_error(SDL_GetError());

	mWindowSurface = SDL_GetWindowSurface(mWindow);

	mBlack = SDL_MapRGB(mWindowSurface->format, 0, 0, 0);
	mWhite = SDL_MapRGB(mWindowSurface->format, 255, 255, 255);

	SDL_SetEventFilter(&SDL2::eventFilter, nullptr);
}

SDL2::~SDL2()
{
	if (mWindow)
		SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

bool SDL2::quit()
{
	SDL_PumpEvents();
	const bool quit = (SDL_HasEvent(SDL_QUIT) == SDL_TRUE);
	SDL_FlushEvent(SDL_QUIT);

	return quit;
}

Key SDL2::waitKey()
{
	SDL_Event event;

	while (SDL_WaitEvent(&event)) {
		if (event.type ==  SDL_KEYDOWN) {
			if (SDLToChip8Key.count(event.key.keysym.scancode) != 0)
				return SDLToChip8Key.at(event.key.keysym.scancode);
		}
		else if (event.type == SDL_QUIT) {
			SDL_PushEvent(&event);
		}
	}

	return Key::KEY_NONE;
}

bool SDL2::keyIsPressed(Key key)
{
	const Uint8* kbState = SDL_GetKeyboardState(nullptr);
	bool pressed = false;

	if (Chip8KeyToSDL.count(key) != 0)
		pressed = kbState[Chip8KeyToSDL.at(key)];

	if (!pressed && Chip8KeyToSDLKeypad.count(key) != 0)
		pressed = kbState[Chip8KeyToSDLKeypad.at(key)];

	return pressed;
}

void SDL2::display(const Display& display)
{
	SDL_LockSurface(mWindowSurface);

	for (unsigned int i = 0; i < Display::WIDTH; ++i) {
		for (unsigned int j = 0; j < Display::HEIGHT; ++j) {
			const Uint32 color = (display.getPixel(i, j)) ? mWhite : mBlack;

			for (unsigned int k = 0; k < PIXELS_PER_CHIP8_PIXEL; ++k) {
				for (unsigned int l = 0; l < PIXELS_PER_CHIP8_PIXEL; ++l) {
					const auto y = i * PIXELS_PER_CHIP8_PIXEL + k;
					const auto x = j * PIXELS_PER_CHIP8_PIXEL + l;

					const Uint8 bpp = mWindowSurface->format->BytesPerPixel;
					Uint8* pixel = reinterpret_cast<Uint8*>(mWindowSurface->pixels) + x * mWindowSurface->pitch + y * bpp;

					colorPixel(pixel, color, bpp);
				}
			}
		}
	}

	SDL_UnlockSurface(mWindowSurface);

	SDL_UpdateWindowSurface(mWindow);
}

void SDL2::colorPixel(Uint8* pixel, Uint32 color, Uint8 bpp)
{
	switch (bpp) {
		case 1:
			*pixel = color;
			break;
		case 2:
			*reinterpret_cast<Uint16*>(pixel) = color;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				pixel[0] = color >> 16;
				pixel[1] = (color >> 8) & 0xff;
				pixel[3] = color & 0xff;
			}
			else {
				pixel[0] = color & 0xff;
				pixel[1] = (color >> 8) & 0xff;
				pixel[3] = color >> 16;
			}
			break;
		case 4:
			*reinterpret_cast<Uint32*>(pixel) = color;
			break;
	}
}

void SDL2::clearDisplay()
{
	SDL_FillRect(mWindowSurface, nullptr, mBlack);
	SDL_UpdateWindowSurface(mWindow);
}

void SDL2::playBeep()
{
	std::cout << __func__ << '\n';
}

std::chrono::milliseconds SDL2::getTicks()
{
	return std::chrono::milliseconds(SDL_GetTicks());
}

void SDL2::delay(const std::chrono::milliseconds& time)
{
	SDL_Delay(time.count());
}

int SDL2::eventFilter(void* userdata, SDL_Event* event)
{
	return (event->type == SDL_KEYDOWN || event->type == SDL_QUIT);
}
