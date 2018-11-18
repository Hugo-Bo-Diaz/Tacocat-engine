#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "CameraComponent.h"

#include "Material.h"

class Mesh
{

public:
	
	uint Resource_UID;

	uint buffer_id;

	float* tex_coords = nullptr;

	uint num_index = 0;
	uint* index = nullptr;

	//uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
	uint num_normals = 0;
	float* normals = nullptr;

	AABB bounding_box;

	bool not_working = false;

	Material* material =nullptr;
	uint materialUID = 0;

	void SetMaterial(Material* mat) { material = mat; }

	bool CheckFrustumCulling(Component_Camera * camera_to_check);
	bool ContainsAaBox(const AABB & refBox, const Frustum& frustum) const;

	void Draw();

	~Mesh();

};

