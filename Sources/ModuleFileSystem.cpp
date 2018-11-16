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

uint ModuleFileSystem::AddResource(Mesh * mesh)
{
	Resource* r = new Resource();
	r->mesh.ptr = mesh;
	r->type = RES_MESH;
	return r->UID;
}

uint ModuleFileSystem::AddResource(Material * mat)
{

	Resource* r = new Resource();
	r->mat.ptr = mat;
	r->type = RES_MATERIAL;
	return r->UID;

}

Resource * ModuleFileSystem::ResourceFromUID(uint UID)
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it)->UID == UID)
			return (*it);
	}
	return nullptr;
}

void ModuleFileSystem::LoadResourcesInfo()
{
}

void ModuleFileSystem::GenerateResourcesInfo(rapidjson::Document * d, rapidjson::Value * v)
{
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{

		rapidjson::Document::AllocatorType& all = d->GetAllocator();

		rapidjson::Value res_obj(rapidjson::kObjectType);

		res_obj.AddMember("UID", (*it)->UID, all);
		res_obj.AddMember("original path",(*it)->path,all);
		res_obj.AddMember("library path", (*it)->path_in_library, all);
		res_obj.AddMember("type", (uint)(*it)->type, all);
		//TODO GET THE FILE DATE AND OTHER INFO that should be in metadata

		v->AddMember("Resource", res_obj, all);

		//LPSYSTEMTIME t;
		//FileTimeToSystemTime(&(*it)->last_modified,t);
		////in t we have the last modification data
		////CreateFile(,);
		//DWORD d;
		//FILE f;
		////OpenFile
		
		

	}
}
