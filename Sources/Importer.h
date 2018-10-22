#pragma once

#define IMPORT_PATH "Library"

class Importer
{
public:
	Importer() {};
	virtual void CreateOwnFile(const char* path_to_file, const char* new_name) { return; };
	virtual void Import(const char* own_file, Scene* scene_to) { return; };

};