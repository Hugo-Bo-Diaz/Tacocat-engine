#include "Globals.h"
#include "Application.h"
#include "ModuleSceneController.h"
#include "Primitive.h"

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
	current_scene = CreateScene("MAIN");

	App->UI->console->AddLog("Loading Intro assets");
	bool ret = true;

	App->scene_controller->current_scene->spookamera->Move(float3(1.0f, 1.0f, 0.0f));
	//App->scene_controller->current_scene->spookamera->frustum.farPlaneDistance = 500;

	for (std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); it++)
	{
		(*it)->Init();
	}

	current_scene->LoadToScene("BakerHouse.fbx");

	App->renderer3D->OnResize(App->window->width, App->window->height);//TODO

	return ret;
}

// Load assets
bool ModuleSceneController::CleanUp()
{

	for (std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); it++)
	{
		(*it)->CleanUp();
		delete(*it);
	}
	scenes.clear();

	return true;
}

// Update
update_status ModuleSceneController::Update(float dt)
{

	current_scene->Update(dt);

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
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		App->importer->mesh->Import();
		App->importer->mesh->LoadCustomMeshFiles(current_scene);
	}


	return UPDATE_CONTINUE;
}

Scene* ModuleSceneController::CreateScene()
{

	Scene* new_scene = new Scene();
	scenes.push_back(new_scene);

	if (current_scene)
		current_scene->Disable();

	current_scene = new_scene;
	current_scene->Enable();

	return new_scene;

}

Scene* ModuleSceneController::CreateScene(const char* name)
{

	Scene* new_scene = new Scene(name);
	scenes.push_back(new_scene);

	if (current_scene)
		current_scene->Disable();
	
	current_scene = new_scene;
	current_scene->Enable();
	
	return new_scene;

}

void ModuleSceneController::DeleteScene(Scene* scene)
{

	scene->CleanUp();

	if (scene == current_scene)
	{
		current_scene->Disable();
		if(!scenes.empty())
			current_scene = *scenes.begin();
	}
		
	std::vector<Scene*>::iterator it;
	for (it = scenes.begin(); it != scenes.end(); it++)
	{
		if (*it == scene)
		{
			break;
		}
	}
	scenes.erase(it);

}

void ModuleSceneController::ChangeScene(Scene* scene_to)
{
	current_scene->Disable();
	current_scene = scene_to;
	current_scene->Enable();
}

void ModuleSceneController::ChangeScene(int id)
{
	Scene* NewCurrentScene = FindSceneById(id);
	current_scene->Disable();
	current_scene = (NewCurrentScene);
	current_scene->Enable();
}

Component_Camera * ModuleSceneController::GetMainCamera()
{
	return current_scene->spookamera;
}

Scene* ModuleSceneController::FindSceneById(int id)
{
	std::vector<Scene*>::iterator it;
	for (it = scenes.begin(); it != scenes.end(); it++)
	{
		if ((*it)->id == id)
		{
			return (*it);
		}
	}

	return nullptr;
}