#pragma once
#include "Module.h"
#include <vector>
#include "Globals.h"
#include "imgui-docking/imgui.h"


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
	Resource();

	Resource(uint _UID);

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

	void Generate_in_Library();

	uint buffer = -1;

};

struct Display
{
	std::string path;
	bool isFolder = false;
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

	uint AddResource(Mesh* mesh, const char* path);
	uint AddResource(Material* mat, const char* path);

	Resource* ResourceFromUID(uint UID);

	Resource* ResourceFromPath(const char* path);

	Resource* LoadFile(const char* path); //should be called always so that way if something exists we know

	//void ReplaceOnLibrary(uint UID);//if you replace a file on the assets folder this can be called so that it generates again in the Library folder

	void LoadResourcesInfo();

	void GenerateResourcesInfo(rapidjson::Document * d, rapidjson::Value * v);

	void DrawUI();

	void RecieveEvent(Event& ev);

	bool window_active = true;

	ImVec2 windowpos; 

	std::string current_directory = "..//Resources"; 
	std::string default_directory = "..//Resources";
	std::vector<Display*> files;

	void GetFilesFromDirectory(const char* directory);

private:

};
