#pragma once

#include <string>
#include "Globals.h"
#include "Component.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

class Component_Material : public Component
{
public:
	uint texture_buffer_id;

	uint tex_width;
	uint tex_height;

	uint material_index = -1;

	Component_Material();

	~Component_Material();

	void Save_Component(JSON_Object*, const char* prev);
	void Load_Component(JSON_Object*, const char* prev);

};