
#pragma once
#include "Color.h"
//#include "MathGeoLib/MathGeoLib.h"
#include "glmath.h"//TOCHANGE

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	vec3 position;//TOCHANGE

	int ref;
	bool on;
};