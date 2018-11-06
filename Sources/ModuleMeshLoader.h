#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
#include "Primitive.h"

#include "MeshComponent.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

#include "pcg32\pcg_variants.h"

//class NOTmesh
//{
//public:
//	uint buffer_id;
//
//	uint texture = 0;
//	float* tex_coords = nullptr;
//
//	//uint id_index = 0; // index in VRAM
//	uint num_index = 0;
//	uint* index = nullptr;
//
//	//uint id_vertex = 0; // unique vertex in VRAM
//	uint num_vertex = 0;
//	float* vertex = nullptr;
//
//	uint num_normals = 0;
//	float* normals = nullptr;
//
//	std::string name;
//
//	aiVector3t<float> scaling;
//	aiVector3t< float > rotation;
//	aiVector3t< float > position;
//
//	AABB bounding_box;
//
//	void draw();
//
//	void draw_bounding_box();
//
//	~NOTmesh();
//
//	void Move(float x, float y, float z)
//	{
//		for (int i = 0; i < num_vertex*3; i += 3)
//		{
//			vertex[i] += x;
//			vertex[i + 1] += y;
//			vertex[i + 2] += z;
//		}
//	}
//
//	void Scale(float scalex,float scaley, float scalez)
//	{
//		for (int i = 0; i < num_index; i += 3)
//		{
//			vertex[i] *= scalex;
//			vertex[i + 1] *= scaley;
//			vertex[i + 2] *= scalez;
//		}
//	}
//
//};

class ModuleMeshLoader : public Module
{
public:
	ModuleMeshLoader(bool start_enabled = true);
	~ModuleMeshLoader();

	bool Start();
	bool CleanUp();
	
	AABB* total_scene_bounding_box = 0;

	void DrawSceneBoundingBox();

	//returns the memory index given by opengl
	void Load_mesh(const char* filename, Scene* scene_to);

	void FocusCamera();
};
