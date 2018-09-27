#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module( start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	name = "Window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	CONSOLE_LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		CONSOLE_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width_to_create = width * SCREEN_SIZE;
		int height_to_create = height * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(full_desktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_to_create, height_to_create, flags);

		if(window == NULL)
		{
			CONSOLE_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	App->UI->console->AddLog("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::Toggle_FullScreen()
{
	if (!fullscreen)
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		SDL_SetWindowSize(window, DM.w, DM.h);

		window_width = width;
		window_height = height;
		width = DM.w;
		height = DM.h;

		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		fullscreen = true;
	}
	else
	{
		SDL_SetWindowSize(window,width, height);
		SDL_SetWindowFullscreen(window, 0);
		width = window_width;
		height = window_height;
		fullscreen = false;
	}
}
void ModuleWindow::Toggle_FullDesktop()
{
	if (!fullscreen)
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		SDL_SetWindowSize(window, DM.w, DM.h);

		window_width = width;
		window_height = height;
		width = DM.w;
		height = DM.h;

		App->renderer3D->OnResize(App->window->width, App->window->height);

		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fullscreen = true;
	}
	else
	{
		SDL_SetWindowSize(window, width, height);
		SDL_SetWindowFullscreen(window, 0);
		width = window_width;
		height = window_height;
		App->renderer3D->OnResize(App->window->width, App->window->height);

		fullscreen = false;
	}
}

void ModuleWindow::Toggle_Borderless()
{
	if (!borderless)
	{
		SDL_SetWindowBordered(App->window->window, SDL_TRUE);
		borderless = true;
	}
	else
	{
		SDL_SetWindowBordered(App->window->window, SDL_FALSE);
		borderless = false;
	}
}

void ModuleWindow::Toggle_Resizable()
{
	if (!resizable)
	{
		
		SDL_SetWindowResizable(window, SDL_TRUE);
		resizable = true;
	}
	else
	{
		SDL_SetWindowResizable(window, SDL_FALSE);
		resizable = false;
	}
}