#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow( bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	bool fullscreen = false;
	bool borderless = false;
	bool full_desktop = false;
	bool resizable = false;
	float brightness = false;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
};

#endif // __ModuleWindow_H__