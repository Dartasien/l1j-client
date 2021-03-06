#ifndef __SDL_BUTTON_H_
#define __SDL_BUTTON_H_

#include <SDL/SDL.h>

#include "funcptr.h"
#include "globals.h"
#include "sdl_widget.h"

sdl_graphic *make_sdl_graphic(int num, int x, int y, sdl_user *who);
SDL_Rect *make_sdl_rect(int x, int y, int w, int h);

class sdl_button : public sdl_widget
{
	public:
		sdl_button(int num, int x, int y, sdl_user *who, funcptr *stuff);
		virtual ~sdl_button();
		
		virtual void cursor_on();
		virtual void cursor_off();
		virtual void key_press(SDL_KeyboardEvent *button);
	protected:
		funcptr *method;

		virtual void mouse_click(SDL_MouseButtonEvent *here);
};

#endif