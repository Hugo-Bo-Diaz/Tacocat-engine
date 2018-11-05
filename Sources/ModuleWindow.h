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
	
	void Toggle_FullScreen();
	void Toggle_FullDesktop();
	void Toggle_Borderless();
	void Toggle_Resizable();

	void Configuration();

	bool conf_fullscreen = false;
	bool conf_resizable = false;
	bool conf_borderless = false;
	bool conf_fulldesktop = false;

	void Save(rapidjson::Document* d, rapidjson::Value* v);

	void Load(rapidjson::Value& v);

	//int width = SCREEN_WIDTH;
	//int height = SCREEN_HEIGHT;

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
	float width = SCREEN_WIDTH;
	float height = SCREEN_HEIGHT;
	float window_width = SCREEN_WIDTH;
	float window_height = SCREEN_HEIGHT;
};

#endif // __ModuleWindow_H__