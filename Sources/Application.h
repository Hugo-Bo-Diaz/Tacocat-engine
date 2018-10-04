#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneController.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleMeshLoader.h"
#include "ModuleTextureLoader.h"
#include "ModuleUI.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneController* scene_controller;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* UI;
	ModuleMeshLoader* mesh_loader;
	ModuleTextureLoader* tex_loader;

	std::list<Module*> list_modules;

private:

	Timer	ms_timer;
	float	dt;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	
	void Close();
	bool quit = false;

	pcg32_random_t rng1, rng2;
	int random_int(int min, int max);
	float random_between_0_1();

	void SaveConfig(const char* filename);
	void LoadConfig(const char* filename);

	double confg_fps = 60.0f;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;
