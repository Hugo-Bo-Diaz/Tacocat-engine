#pragma once

#include <string>
#include "Globals.h"
#include "Component.h"
#include "MaterialComponent.h"
#include "Mesh.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

class Component_Camera;

class Component_Mesh : public Component
{
public:
	
	uint resource_id;

	Component_Mesh();

	//aiVector3t<float> current_scaling = {1,1,1};
	//aiQuaternion rotation_q;
	//aiVector3t<float> current_rotation = {0,0,0};
	//aiVector3t< float > current_translation = {0,0,0};

	Mesh* mesh;

	AABB bounding_box;
	void draw_bounding_box();
	//void generate_buffer();
	void Update(float dt);

	~Component_Mesh();
	bool ContainsAaBox(const AABB & refBox, const Frustum& frustum) const;

	bool CheckFrustumCulling(Component_Camera* camera_to_check);

	void Save_Component(rapidjson::Document* d, rapidjson::Value* v);
	void Load_Component(rapidjson::Value::ConstMemberIterator v);

	void Properties();

	Component_Material* material = nullptr;

	bool not_working = false;

};