#include "ModuleFileSystem.h"
#include "Application.h"

ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
	listentoevents = true;
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

void ModuleFileSystem::LoadResourcesInfo()
{
}

void ModuleFileSystem::GenerateResourcesInfo()
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		(*it)->UID;
		LPSYSTEMTIME t;
		FileTimeToSystemTime(&(*it)->last_modified,t);
		//in t we have the last modification data
		(*it)->path;
		(*it)->path_in_library;
		(*it)->type;
		CreateFile(,);
		DWORD d;
		FILE f;
		OpenFile
		
		

	}
}
