#include "draw_loading.h"

#include "client.h"
#include "globals.h"
#include "resources/prepared_graphics.h"
#include "sdl_user.h"
#include "widgets/sdl_animate_button.h"
#include "widgets/sdl_input_box.h"
#include "widgets/sdl_text_button.h"
#include "widgets/text_box.h"

load_ptr::load_ptr(draw_loading *bla, int val)
{
	ref = bla;
	which = val;
}

void load_ptr::go()
{
	ref->server_picked(which);
}

draw_loading::draw_loading(sdl_user *self)
	: sdl_drawmode(self)
{
	owner->game_music.change_music("sound/music0.mp3");
	load_progress = 0;
	load_amount = 0x249f0;	//not sure how this number is generated
	pg = new prepared_graphics;
	pg->num_pg = 2;
	
	int index;
	SDL_Rect *rect;

	pg->pg = new prepared_graphic[2];
	
	pg->pg[0].surf = get_png_image(811, owner->game);
	pg->pg[0].mask = NULL;
	pg->pg[0].position = NULL;
	pg->pg[0].cleanup = false;
	
	pg->pg[1].surf = get_img(330, owner->game);
	rect = new SDL_Rect;
  	rect->x = 0xf1;
  	rect->y = 0x181;
	pg->pg[1].position = rect;
	rect = new SDL_Rect;
	rect->x = 0;
	rect->y = 0;
	rect->h = pg->pg[1].surf->h;
	rect->w = 0;
	pg->pg[1].mask = rect;
	pg->pg[1].cleanup = true;
	pg->ready = true;
	
	num_servers = owner->game->get_config()->get_num_servers();
	
	num_widgets = num_servers + 1;
	widgets = new sdl_widget*[num_widgets];
	
	server_pick = -1;
	spick_mtx = SDL_CreateMutex();
	
	widget_key_focus = 0;
	for (int i = 0; i < num_servers; i++)
	{
		widgets[i] = new sdl_text_button(owner->game->get_config()->get_name(i), 276, 254+(15*(i+1)), owner->game, 
			(funcptr*)new load_ptr(this, i));
		widgets[i]->set_key_focus(true);
	}
	widgets[num_servers] = new text_box(257, 254, 150, 12, owner->game);
	
	((text_box*)widgets[num_servers])->add_line("Please select a server:");
		
	update_load();
}

int draw_loading::wait_server_pick()
{
	bool done = false;
	
	while (!done)
	{
		while (SDL_mutexP(spick_mtx) == -1) {};
		if (server_pick != -1)
			done = true;
		SDL_mutexV(spick_mtx);
		if (!done)
		{
			SDL_Delay(100);
		}
	}
	return server_pick;
}

void draw_loading::server_picked(int i)
{
	while (SDL_mutexP(spick_mtx) == -1) {};
	server_pick = i;
	SDL_mutexV(spick_mtx);
}

void draw_loading::set_load_amount(int size)
{
	load_amount = size;
}

void draw_loading::update_load()
{
	float temp = (float)pg->pg[1].surf->w * ((float)load_progress / (float)load_amount);
	pg->pg[1].mask->w = (Uint16)temp;
}

void draw_loading::add_loaded(int size)
{
	load_progress += size;
	update_load();
}

void draw_loading::load_done()
{
	load_progress = load_amount;
	update_load();
	SDL_Delay(250);
	owner->change_drawmode(DRAWMODE_LOGIN);
}

draw_loading::~draw_loading()
{
}

bool draw_loading::mouse_leave()
{
	return false;
}

void draw_loading::mouse_click(SDL_MouseButtonEvent *here)
{
	if (num_widgets > 0)
	{
		int index = get_widget(here->x, here->y);
		if (index != -1)
		{
			widgets[index]->mouse_click(here);
		}
	}
}

void draw_loading::mouse_to(SDL_MouseMotionEvent *to)
{
	int which = get_widget(to->x, to->y);
	if (which >= 0)
	{
		widgets[which]->mouse_to(to);
	}
}

void draw_loading::mouse_from(SDL_MouseMotionEvent *from)
{
	int which = get_widget(from->x, from->y);
	if (which >= 0)
	{
		widgets[which]->mouse_from(from);
	}
}

void draw_loading::mouse_move(SDL_MouseMotionEvent *from, SDL_MouseMotionEvent *to)
{	//TODO: handle click and drag movable widgets
	if (num_widgets > 0)
	{
		int from_w = get_widget(from->x, from->y);
		int to_w = get_widget(to->x, to->y);
		if ((from_w != -1) && (to_w != -1))
		{
			if (from_w != to_w)
			{	//only send events if the widgets are different
				mouse_from(from);
				mouse_to(to);
			}
		}
		if ((from_w == -1) && (to_w != -1))
		{
			mouse_to(to);
		}
		if ((from_w != -1) && (to_w == -1))
		{
			mouse_from(from);
		}
	}
}