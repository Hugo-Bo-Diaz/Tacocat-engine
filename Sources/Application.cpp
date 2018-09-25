#include "Application.h"
#include "parson/parson.h"

Application::Application()
{

	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	audio = new ModuleAudio(true);
	scene_controller = new ModuleSceneController(true);
	renderer3D = new ModuleRenderer3D(true);
	camera = new ModuleCamera3D(true);
	physics = new ModulePhysics3D(true);
	UI = new ModuleUI(true);

	pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);
	pcg32_srandom_r(&rng2, time(NULL), (intptr_t)&rng2);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_controller);
	
	// Renderer last!
	AddModule(renderer3D);

	// UI
	AddModule(UI);


}

Application::~Application()
{
	//std::list<Module*>::iterator item = list_modules.end();

	for (std::list<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend(); it++)
	{
		//list_modules.remove(*it);
		delete (*it);
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	LoadConfig("config.json");

	// Call Init() in all modules

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; it++)
	{
		ret = (*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	CONSOLE_LOG("Application Start --------------");

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; it++)
	{
		ret = (*it)->Start();
	}
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->Update(dt);
	}

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend(); it++)
		if (!(*it)->CleanUp()) return false;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::Close()
{
	quit = true;
}

int Application::random_int(int min, int max)
{
	return (int)pcg32_boundedrand_r(&rng1, max - min) + min;
}

float Application::random_between_0_1()
{
	return ldexp(pcg32_random_r(&rng2), -32);
}

void Application::SaveConfig(const char* filename)
{
	JSON_Value *config = json_parse_file("config.json");
	config = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(config);

	json_object_set_number(root_object, "fps", confg_fps);
	
	json_object_dotset_number(root_object, "Audio.volume", App->audio->volume);

	json_object_dotset_boolean(root_object, "Window.fullscreen", App->window->fullscreen);
	json_object_dotset_boolean(root_object, "Window.borderless", App->window->borderless);
	json_object_dotset_boolean(root_object, "Window.resizable", App->window->resizable);
	json_object_dotset_boolean(root_object, "Window.full_desktop", App->window->full_desktop);
	json_object_dotset_number(root_object, "Window.brightness", App->window->brightness);
	json_object_dotset_number(root_object, "Window.width", App->window->width);
	json_object_dotset_number(root_object, "Window.height", App->window->height);

	json_serialize_to_file(config, "config.json");

}

void Application::LoadConfig(const char* filename)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(filename);
	if (json_value_get_type(root_value) != JSONObject) {
		CONSOLE_LOG("couldn't find the file %d", filename);
		return;
	}
	root_object = json_value_get_object(root_value);

	confg_fps = json_object_get_number(root_object, "fps");

	App->audio->volume = json_object_dotget_number(root_object, "Audio.volume");

	App->window->fullscreen = json_object_dotget_boolean(root_object, "Window.fullscreen");
	App->window->borderless = json_object_dotget_boolean(root_object, "Window.borderless");
	App->window->resizable = json_object_dotget_boolean(root_object, "Window.resizable");
	App->window->full_desktop = json_object_dotget_boolean(root_object, "Window.full_desktop");
	App->window->brightness = json_object_dotget_number(root_object, "Window.brightness");
	App->window->width = json_object_dotget_number(root_object, "Window.width");
	App->window->height = json_object_dotget_number(root_object, "Window.height");

}