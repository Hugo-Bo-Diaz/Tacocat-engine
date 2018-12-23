#include "Scene.h"
#include "Application.h"
#include "ModuleUI.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

#include "Wwise_IDs.h"

Scene::~Scene()
{

	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		delete (*it);
	}
	GameObjects.clear();

}

void Scene::Init()
{
	GameObject* f = AddGameObject();
	Component_Transform* t = new Component_Transform();
	t->rotation.Set(0, 0, 0, 0);
	t->position.Set(0, 0, 0);
	t->scaling.Set(0, 0, 0);
	f->AddComponent(t);
	Component_Audio_Emitter* p = new Component_Audio_Emitter();
	f->AddComponent(p);
	p->SetWwiseObject();


	Camera = AddGameObject();
	Component_Transform* s = new Component_Transform();
	s->rotation.Set(0, 0, 0, 0);
	s->position.Set(0, 0, 0);
	s->scaling.Set(0, 0, 0);
	Camera->AddComponent(s);
	Camera->AddComponent(spookamera);
	Component_Audio_Listener* l = new Component_Audio_Listener();
	Camera->AddComponent(l);
	l->SetWwiseObject();

	AK::SoundEngine::RegisterGameObj(music_player,"player");
	AK::SoundEngine::PostEvent(AK::EVENTS::MUSIC, music_player);

	//DEMO OBJECTS
	Demo_static = AddGameObject();
	Demo_static->name = "Demo_static";
	Component_Transform* b_static = new Component_Transform();
	b_static->rotation.Set(0, 0, 0, 0);
	b_static->position.Set(0, 0, -50);
	b_static->scaling.Set(0, 0, 0);
	Demo_static->AddComponent(b_static);

	Component_Audio_Emitter* x_static = new Component_Audio_Emitter();
	Demo_static->AddComponent(x_static);
	x_static->SetWwiseObject();
	x_static->PlayEvent(AK::EVENTS::LOOP_STATIC);

	Demo_static->bounding_box.minPoint = float3(-1,-1,-1);
	Demo_static->bounding_box.maxPoint = float3(1, 1, 1);

	Demo_not_static = AddGameObject();
	Demo_not_static->name = "Demo_moving";
	Component_Transform* b_not_static = new Component_Transform();
	b_not_static->rotation.Set(0, 0, 0, 0);
	b_not_static->position.Set(0, 0, -70);
	b_not_static->scaling.Set(0, 0, 0);
	Demo_not_static->AddComponent(b_not_static);

	Component_Audio_Emitter* x_not_static = new Component_Audio_Emitter();
	Demo_not_static->AddComponent(x_not_static);
	x_not_static->SetWwiseObject();
	x_not_static->PlayEvent(AK::EVENTS::LOOP_MOVING);

	Demo_not_static->bounding_box.minPoint = float3(-1, -1, -1);
	Demo_not_static->bounding_box.maxPoint = float3(1, 1, 1);
}

void Scene::Update(float dt)
{
	//DEMO THINGS

		Demo_static->bounding_box.minPoint = float3(Demo_static->GetTransformComponent()->position.x - 1, Demo_static->GetTransformComponent()->position.y - 1, Demo_static->GetTransformComponent()->position.z - 1);
		Demo_static->bounding_box.maxPoint = float3(Demo_static->GetTransformComponent()->position.x + 1, Demo_static->GetTransformComponent()->position.y + 1, Demo_static->GetTransformComponent()->position.z + 1);
		Demo_not_static->bounding_box.minPoint = float3(Demo_not_static->GetTransformComponent()->position.x - 1, Demo_not_static->GetTransformComponent()->position.y - 1, Demo_not_static->GetTransformComponent()->position.z - 1);
		Demo_not_static->bounding_box.maxPoint = float3(Demo_not_static->GetTransformComponent()->position.x + 1, Demo_not_static->GetTransformComponent()->position.y + 1, Demo_not_static->GetTransformComponent()->position.z + 1);
		
	if (Demo_not_static->GetTransformComponent()->position.x < 50 && going_positive)
	{
		Demo_not_static->GetTransformComponent()->position.x += 10*dt;
		Demo_not_static->GetTransformComponent()->Caluculate_Local_Matrix();
		if (Demo_not_static->GetTransformComponent()->position.x >= 50)
		{
			going_positive = false;
		}
	}

	if (Demo_not_static->GetTransformComponent()->position.x > -50 && !going_positive)
	{
		Demo_not_static->GetTransformComponent()->position.x -= 10 * dt;
		Demo_not_static->GetTransformComponent()->Caluculate_Local_Matrix();
		if (Demo_not_static->GetTransformComponent()->position.x <= -50)
		{
			going_positive = true;
		}
	}



	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		(*it)->CalculateAllTransformMatrices(float4x4::identity);
		(*it)->Update(dt);
	}

	if (App->renderer3D->conf_draw == 1 || App->renderer3D->conf_draw == 2)
	{
		for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
		{
			(*it)->DrawBoundingBox();
		}
	}



	//spookamera->Update(dt);

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
	////std::string extension = std::strrchr(file, '.');
	////if (extension == ".FBX" || extension == ".fbx")
	////{
	////	App->UI->console->AddLog("model file detected, sending to mesh loader");
	////	App->mesh_loader->Load_mesh(file, this); 
	////}
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
	//tree->IntoSpooktree(object);
	return object;
}

GameObject* Scene::GetObjectFromUID(uint UID)
{
	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
	{
		if ((*it)->UID == UID)
		{
			return *it;
		}
	}
	return nullptr;
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

	App->fsys->GenerateResourcesInfo(&document, &object_node);

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

	rapidjson::Document file;

	FILE* f = fopen(filename, "rb");
	if (f)
	{
		char Buffer[65536];
		rapidjson::FileReadStream input(f, Buffer, sizeof(Buffer));
		file.ParseStream(input);

		const rapidjson::Value& scene = file["Scene"];

		for (rapidjson::Value::ConstMemberIterator itr = scene.MemberBegin(); itr != scene.MemberEnd(); itr++)
		{
			if (itr->name == "OBJECT")
			{
				GameObject* obj = AddGameObject();
				obj->UID = itr->value["UID"].GetInt();
				obj->parent_uid = itr->value["parentUID"].GetInt();

				const rapidjson::Value& components = itr->value["Components"];

				for (rapidjson::Value::ConstMemberIterator comp = components.MemberBegin(); comp != components.MemberEnd(); comp++)
				{
					if (comp->name == "TRANSFORM")
					{
						Component_Transform* t = new Component_Transform();
						t->Load_Component(comp);
						obj->AddComponent(t);
					}
					else if (comp->name == "MESH")
					{
						Mesh* m;
						Component_Mesh* cm = new Component_Mesh();
						cm->Load_Component(comp);
						obj->AddComponent(cm);
					}
					else if (comp->name == "MATERIAL")
					{
						Material* m;
						Component_Material* cmat = new Component_Material();
						cmat->Load_Component(comp);
						obj->AddComponent(cmat);


						break;
					}
				}
			}
			if (itr->name == "Resource")
			{

			}
		}

		std::vector<GameObject*> indexestoeliminate;
		for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); it++)
		{
			if ((*it)->parent_uid != 0)
			{
				GetObjectFromUID((*it)->parent_uid)->AddChild(*it);
				indexestoeliminate.push_back(*it);
			}
		}
		
		for (std::vector<GameObject*>::iterator it_1 = indexestoeliminate.begin(); it_1 != indexestoeliminate.end(); ++it_1)
		{
			std::vector<GameObject*>::iterator u = std::find(GameObjects.begin(),GameObjects.end(),*it_1);
			GameObjects.erase(u);
		}

	}
}

