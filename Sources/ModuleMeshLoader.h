#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
#include "Primitive.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "pcg32\pcg_variants.h"

#define MAX_SNAKE 2

class NOTmesh : public NOTprimitive
{
public:
	uint buffer_id;

	uint texture;
	float* tex_coords;

	//uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	//uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	void draw();

	void Scale(float scalex,float scaley, float scalez)
	{
		for (int i = 0; i < num_index; i += 3)
		{
			vertex[i] *= scalex;
			vertex[i + 1] *= scaley;
			vertex[i + 2] *= scalez;
		}
	}

};

class ModuleMeshLoader : public Module
{
public:
	ModuleMeshLoader(bool start_enabled = true);
	~ModuleMeshLoader();

	bool Start();
	bool CleanUp();

	//returns the memory index given by opengl
	uint* Load(const char* filename);
public:

};
