#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
#include "Material.h"


#include "pcg32\pcg_variants.h"

class ModuleTextureLoader : public Module
{
public:
	ModuleTextureLoader(bool start_enabled = true);
	~ModuleTextureLoader();

	bool Start();
	bool CleanUp();

	Material* LoadTexture(const char* file);

	void RecieveEvent(Event& ev);

public:
	int texture_id = 0;
	int num_meshes = 0;
	int tex_width = 0, tex_height = 0;
};
