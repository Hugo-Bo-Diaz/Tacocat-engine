#pragma once

#include <string>
#include "Globals.h"
#include "Component.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"

#include "MathGeoLib/MathGeoLib.h"

class Component_Camera : public Component
{
public:

	Frustum frustum;

	bool culling;

	float3 X, Y, Z, Position, Reference;

	float3 newPos = {0,0,0};

	Component_Camera();

	void Update(float dt);

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	void CalculateViewMatrix();

	float3 Rotate(const float3 &u, float angle, const float3 &v)
	{
		return *(float3*)&(float4x4::RotateAxisAngle( v,angle) * float4(u, 1.0f));
	}
	float speed = 0.1;

	float sensibility = 1;

	float ratio;
	float fov_x;
	float fov_y = 60;

	float angle_XZ = 0;
	float angle_Y = 0;
	
	//this is for the frustum geometry
	float* vertices;
	uint* indices;
	uint buffer_id;

	void Generate_frustum();
	void Draw_frustum();
	

private:

	float4x4 ViewMatrix, ViewMatrixInverse;


	~Component_Camera() {};

};	
