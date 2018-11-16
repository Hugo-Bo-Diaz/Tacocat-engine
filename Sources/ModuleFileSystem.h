#pragma once
#include "Module.h"
#include <vector>
#include "Globals.h"

#include "Mesh.h"
#include "Material.h"

enum ResourceType
{
	RES_MESH,
	RES_MATERIAL,
	RES_AUDIO,
	RES_INVALID
};

struct Resource
{
	Resource()
	{
		UID = App->random_int(0, 100000);
	}
	Resource(uint _UID)
	{
		UID = _UID;
	}

	uint UID;
	std::string path;
	std::string path_in_library;

	FILETIME last_modified;

	ResourceType type;

	union
	{
		struct
		{
			Mesh* ptr = nullptr;
		} mesh;
		struct
		{
			Material* ptr = nullptr;
		} mat;
	};

	void Generate_in_Library()
	{
		if (mesh.ptr != nullptr)
		{
			switch ((uint)type)
			{
			case RES_MESH:
				App->importer->mesh->CreateOwnFile(mesh.ptr);
				break;
			case RES_MATERIAL:
				;
				break;
			case RES_AUDIO:
				App->UI->console->AddLog("audio not implemented");
				break;
			case RES_INVALID:
				App->UI->console->AddLog("not a valid resource type");
			default:
				break;
			}
		}
	}

	uint buffer = -1;

};

class ModuleFileSystem: public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	std::vector<Resource*> resources;
	std::list<std::string> paths_opened;

	bool Start();
	bool CleanUp();

	void Save(rapidjson::Document* d, rapidjson::Value* v);
	void Load(rapidjson::Value& v);

	uint AddResource(Mesh* mesh);
	uint AddResource(Material* mat);

	Resource* ResourceFromUID(uint UID);

	Resource* LoadFile(const char* path); //should be called always so that way if something exists we know

	//void ReplaceOnLibrary(uint UID);//if you replace a file on the assets folder this can be called so that it generates again in the Library folder

	void LoadResourcesInfo();

	void GenerateResourcesInfo(rapidjson::Document * d, rapidjson::Value * v);

	//void DrawOnUI();

private:

};
