#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"

#include "MeshImporter.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	MeshImporter* mesh = nullptr;
	
	bool Start();
	bool CleanUp();

	void Load(const char* filename, Scene* scene_to);

	void FocusCamera();

private:

};