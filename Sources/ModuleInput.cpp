#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include <string.h>

#define MAX_KEYS 300

ModuleInput::ModuleInput( bool start_enabled) : Module( start_enabled)
{
	name = "Input";

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Start()
{
	App->UI->console->AddLog("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);



	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->UI->console->AddLog("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				if (sendinputs)App->UI->console->AddLog("Keyboard - %d - DOWN", i);
				keyboard[i] = KEY_DOWN;
			}
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				if (sendinputs)App->UI->console->AddLog("Keyboard - %d - UP", i);
				keyboard[i] = KEY_UP;
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
			{
				mouse_buttons[i] = KEY_DOWN;
				if (sendinputs)App->UI->console->AddLog("Mouse - %d - DOWN",i);
			}
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
			{
				mouse_buttons[i] = KEY_UP;
				if (sendinputs)App->UI->console->AddLog("Mouse - %d - UP", i);

			}
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			App->scene_controller->current_scene->spookamera->newPos -= App->scene_controller->current_scene->spookamera->Z * e.wheel.y * 30 * dt;//TODO
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
				App->Close();
			break;

			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					//App->renderer3D->OnResize(e.window.data1, e.window.data2);
					//App->window->width = e.window.data1;
					//App->window->height = e.window.data2;

					Event* ev = new Event(Event::window_resize);
					ev->point2d.x = e.window.data1;
					ev->point2d.y = e.window.data2;
					App->BroadcastEvent(*ev);
				}
			}
			break;

			case SDL_DROPFILE:
				App->UI->console->AddLog("File dropped in window %s", e.drop.file);
				
				//App->importer->Load(e.drop.file, App->scene_controller->current_scene);
				Event* ev = new Event(Event::file_dropped);
				ev->string.ptr = e.drop.file;
				App->BroadcastEvent(*ev);

			break;
		}
	}
	

	if (App->quit)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->UI->console->AddLog("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::Configuration()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position: %d, %d", GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse position: %d, %d", GetMouseXMotion(), App->input->GetMouseYMotion());
		ImGui::Checkbox("Send inputs to console", &sendinputs);
	}
}

