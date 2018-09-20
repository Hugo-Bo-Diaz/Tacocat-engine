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
#include "ModulePhysics3D.h"
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
	ModulePhysics3D* physics;
	ModuleUI* UI;

	LCG* random;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	
	void Close();
	bool quit = false;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;
