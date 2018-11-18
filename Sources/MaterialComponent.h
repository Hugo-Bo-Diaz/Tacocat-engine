#pragma once

#include <string>
#include "Globals.h"
#include "Component.h"
#include "Material.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

class Component_Material : public Component
{
public:

	uint resource_UID;
	Material* material;
	
	Component_Material();

	Material* GetMaterial()
	{
		return material;
	}

	~Component_Material();

	void Save_Component(rapidjson::Document* d, rapidjson::Value* v);
	void Load_Component(rapidjson::Value::ConstMemberIterator v);

};