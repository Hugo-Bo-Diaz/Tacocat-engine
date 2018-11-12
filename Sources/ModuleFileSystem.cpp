#include "ModuleFileSystem.h"
#include "Application.h"

ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

void ModuleFileSystem::Save(rapidjson::Document * d, rapidjson::Value * v)
{
}

void ModuleFileSystem::Load(rapidjson::Value & v)
{
}
