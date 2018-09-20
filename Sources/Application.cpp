#include "Application.h"

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