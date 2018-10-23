#include "MeshImporter.h"

void MeshImporter::CreateOwnFile(const char * path_to_file, const char * new_name)
{
	std::string extension = std::strrchr(path_to_file, '.');
	if (extension == ".FBX" || extension == ".fbx")
	{
		//serialize
		
	}
}

void MeshImporter::Import(const char * path_file, Scene * scene_to)
{
}
