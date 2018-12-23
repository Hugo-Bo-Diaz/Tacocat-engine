#pragma once

#include <string>
#include "Globals.h"
#include "Component.h"
#include "MaterialComponent.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"


class Component_Transform : public Component
{
public:

	//uint id_vertex = 0; // unique vertex in VRAM
	//uint num_vertex = 0;
	//float* vertex = nullptr;
	//uint num_normals = 0;
	//float* normals = nullptr;

	Component_Transform();

	aiVector3t<float> scaling;
	Quat rotation;
	aiVector3t<float> rotation_angle;
	aiVector3t< float > position;

	float3 GetGlobalPosition();

	float4x4 transform_local = float4x4::identity;//give this away to calculate other matrices
	float4x4 transform_global = float4x4::identity;//use this for transformation in meshes

	void Calculate_Global_Matrix(float4x4 globalmatrixoftheobject);
	void Caluculate_Local_Matrix();

	void Rotate(float angle_x, float angle_y, float angle_z);

	~Component_Transform();

	void Save_Component(rapidjson::Document* d, rapidjson::Value* v);
	void Load_Component(rapidjson::Value::ConstMemberIterator v);

	void Properties();
};