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

	bool Start();
	bool CleanUp();

	void Save(rapidjson::Document* d, rapidjson::Value* v);
	void Load(rapidjson::Value& v);

	//void LoadFile();

	//uint ResourceFromUID(uint UID);

	//void ReplaceOnLibrary(uint UID);//if you replace a file on the assets folder this can be called so that it generates again in the Library folder

	void LoadResourcesInfo();

	void GenerateResourcesInfo();

	//void DrawOnUI();

private:

};
