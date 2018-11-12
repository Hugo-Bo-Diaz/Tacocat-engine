#include "TransformComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

Component_Transform::Component_Transform()
{
	type = TRANSFORM;
}

float3x3 Component_Transform::Get_Transform_Matrix()
{

	float3x3 final_matrix;

	float3x3 scale_matrix;

	float3x3 rotation_matrix;
	aiVector3D euler_rotation = rotation.GetEuler();

	float4x4::FromTRS();
	float4x4 t;
	glPushMatrix();

	final_matrix = scale_matrix * rotation_matrix;

	return final_matrix;
}

void Component_Transform::Translate_vertices(float * vertices, uint number_of_vertices)
{

	

}

Component_Transform::~Component_Transform()
{

}

void Component_Transform::Save_Component(rapidjson::Document* d, rapidjson::Value* v)
{

}

void Component_Transform::Load_Component(rapidjson::Value& v)
{

}