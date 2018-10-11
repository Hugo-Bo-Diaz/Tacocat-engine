#pragma once
#include "Module.h"
#include "Globals.h"
//#include "glmath.h"//TOCHANGE


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	//void LookAt(const float3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	//float3 X, Y, Z, Position, Reference;
	vec3 X, Y, Z, Position, Reference;

	vec3 newPos; 
private:

	//float4x4 ViewMatrix, ViewMatrixInverse;
	mat4x4 ViewMatrix, ViewMatrixInverse;

};