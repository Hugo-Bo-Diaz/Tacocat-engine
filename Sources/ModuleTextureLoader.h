#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"

#include "pcg32\pcg_variants.h"

class ModuleTextureLoader : public Module
{
public:
	ModuleTextureLoader(bool start_enabled = true);
	~ModuleTextureLoader();

	bool Start();
	bool CleanUp();

	uint LoadTexture(const char* file);

public:

};
