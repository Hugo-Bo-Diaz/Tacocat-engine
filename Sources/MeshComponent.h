#pragma once

#include <string>
#include "Globals.h"
#include "Component.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

class Component_Camera;

class Component_Mesh : public Component
{
public:
	uint buffer_id;

	float* tex_coords = nullptr;

	//uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	//uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
	uint num_normals = 0;
	float* normals = nullptr;

	std::string name;

	uint material_index = -1;

	Component_Mesh();

	aiVector3t<float> scaling;
	aiVector3t< float > rotation;
	aiVector3t< float > position;
	AABB bounding_box;
	void draw();
	void draw_bounding_box();
	//void generate_buffer();
	void Update(float dt);

	~Component_Mesh();
	void Move(float x, float y, float z);

	void Scale(float scalex, float scaley, float scalez);

	bool CheckFrustumCulling(Component_Camera* camera_to_check);

	//float imdumb = 0;

};