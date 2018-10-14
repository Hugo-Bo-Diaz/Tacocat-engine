#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"

#include "pcg32\pcg_variants.h"

//todo TEXTURE STRUCT


class ModuleTextureLoader : public Module
{
public:
	ModuleTextureLoader(bool start_enabled = true);
	~ModuleTextureLoader();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	uint LoadTexture(const char* file);

public:
	int texture_id = 0;
	int num_meshes = 0;
	int tex_width = 0, tex_height = 0;
};
