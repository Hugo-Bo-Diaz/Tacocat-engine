#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleMeshLoader.h"

ModuleImporter::ModuleImporter(bool start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	mesh = new MeshImporter();
	return true;
}

bool ModuleImporter::CleanUp()
{
	return true;
}

void ModuleImporter::Load(const char * filename, Scene * scene_to)
{
	std::string extension = std::strrchr(filename, '.');
	if (extension == ".FBX" || extension == ".fbx")
	{
		App->mesh_loader->Load_mesh(filename,scene_to);
	//	mesh->CreateOwnFile(filename,nullptr);
	}
}
