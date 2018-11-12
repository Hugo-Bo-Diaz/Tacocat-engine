#pragma once
#include "Module.h"
#include <vector>
#include "Globals.h"

struct Resource
{
	uint UID;
	std::string path;

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

private:

};
