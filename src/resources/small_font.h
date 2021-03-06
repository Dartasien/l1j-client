#ifndef __SMALL_FONT_H_
#define __SMALL_FONT_H_

#include "sdl_font.h"
class sdl_user;

#define COLORKEY 255, 0, 255 //Your Transparent colour

class small_font : public sdl_font
{
	public:
		small_font();
		~small_font();
		virtual bool init(const char *name, sdl_user *who);
		virtual int draw(SDL_Surface *surf, int x, int y, char val, Uint32 color);
		virtual int draw(SDL_Surface *surf, int x, int y, const char *str, Uint32 color);
		virtual void draw_cursor(SDL_Surface *surf, int x, int y, Uint32 color);
	private:
		SDL_Surface *final;
		char convert[256];
};

#endif