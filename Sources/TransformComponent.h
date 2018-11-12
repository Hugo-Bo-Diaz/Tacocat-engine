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
	aiQuaternion rotation;
	aiVector3t< float > position;

	float3x3 Get_Transform_Matrix();
	void Translate_vertices(float* vertices, uint number_of_vertices);

	~Component_Transform();

	void Save_Component(rapidjson::Document* d, rapidjson::Value* v);
	void Load_Component(rapidjson::Value& v);

};