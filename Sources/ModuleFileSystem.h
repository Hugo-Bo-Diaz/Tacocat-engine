#pragma once
#include "Module.h"
#include <vector>
#include "Globals.h"

#include "Mesh.h"
#include "Material.h"

enum ResourceType
{
	RES_MESH,
	RES_TEXTURE,
	RES_AUDIO
};

struct Resource
{
	uint UID;
	std::string path;
	std::string path_in_library;

	ResourceType type;

	union
	{
		struct
		{
			Mesh* ptr;
		} mesh;
		struct
		{
			Material* ptr;
		} mat;
	};

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

	//void LoadResourcesInfo();

	//void DrawOnUI();

private:

};
