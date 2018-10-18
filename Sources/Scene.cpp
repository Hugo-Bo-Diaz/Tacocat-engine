#include "Scene.h"
#include "Application.h"
#include "ModuleUI.h"

Scene::~Scene()
{

	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		delete (*it);
	}
	GameObjects.clear();

}

void Scene::Update(float dt)
{
	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		(*it)->Update(dt);
	}
	spookamera->Update(dt);
}

void Scene::LoadToScene(const char* file)
{
	GameObject* root = new GameObject();

	//TODO
	//FIRST: check the file extension
	std::string extension = std::strrchr(file, '.');
	if (extension == ".FBX" || extension == ".fbx")
	{
		App->UI->console->AddLog("model file detected, sending to mesh loader");
		App->mesh_loader->Load(file, this); 
	}
	//else if (extension == ".png" || extension == ".dss")
	//{
	//	App->UI->console->AddLog("image file detected, sending to texture loader");
	//	App->tex_loader->LoadTexture(file);
	//}
	//SECOND: call the correspondant loader
	//THIRD: generate the gameobjects needed
	//FOURTH: profit

}

GameObject* Scene::AddGameObject()
{
	GameObject* object = new GameObject();
	GameObjects.push_back(object);
	return object;
}
