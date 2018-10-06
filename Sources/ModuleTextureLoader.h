#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"

#include "pcg32\pcg_variants.h"

#define MAX_SNAKE 2
//todo TEXTURE STRUCT


class ModuleTextureLoader : public Module
{
public:
	ModuleTextureLoader(bool start_enabled = true);
	~ModuleTextureLoader();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

public:

};
