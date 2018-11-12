#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"


class ModuleFileSystem: public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool Start();
	bool CleanUp();

private:

};
