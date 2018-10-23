#include "ModuleImporter.h"

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
	//std::string extension = std::strrchr(filename, '.');
	//if (extension == ".FBX" || extension == ".fbx")
	//{
	//	mesh->CreateOwnFile(filename,nullptr);
	//}
}
