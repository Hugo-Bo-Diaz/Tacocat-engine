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

	tree->Draw();

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		Save("scenelol.json");
	}
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
	tree->IntoSpooktree(object);
	return object;
}

void Scene::Save(const char * filename)
{
	rapidjson::Document document;
	document.SetObject();
	FILE* fp = fopen(filename, "wb");
	char writeBuffer[655360];

	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document::AllocatorType& all = document.GetAllocator();

	rapidjson::Value object_node(rapidjson::kObjectType);

	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		(*it)->Save(&document,&object_node);
	}

	document.AddMember("Scene", object_node, all);
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);

	//JSON_Value *config = json_parse_file(filename);
	//config = json_value_init_object();
	//JSON_Object* root_object = json_value_get_object(config);

	//json_object_set_string(root_object, "Name", name.c_str());

	//for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	//{
	//	(*it)->Save(".Objects",root_object);
	//}

	//json_serialize_to_file(config, filename);
}

void Scene::Load(const char * filename)
{
}
