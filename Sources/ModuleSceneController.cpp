#include "Globals.h"
#include "Application.h"
#include "ModuleSceneController.h"
#include "Primitive.h"
#include "PhysBody3D.h"

//#include "mathgeolib/Geometry/Sphere.h"
//#include "mathgeolib/Geometry/AABB.h"

#include "parson\parson.h"

//#include "mmgr\mmgr.h"

ModuleSceneController::ModuleSceneController(bool start_enabled) : Module(start_enabled)
{
	name = "SceneController";
}

ModuleSceneController::~ModuleSceneController()
{}

// Load assets
bool ModuleSceneController::Start()
{
	App->UI->console->AddLog("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));

	App->mesh_loader->Load("BakerHouse.fbx");

	return ret;
}

// Load assets
bool ModuleSceneController::CleanUp()
{
	App->UI->console->AddLog("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneController::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		App->mesh_loader->FocusCamera();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		App->SaveConfig("config.json");
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		App->LoadConfig("config.json");
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneController::Draw()
{

	
}
