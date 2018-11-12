#include "TransformComponent.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

Component_Transform::Component_Transform()
{
	type = TRANSFORM;

}

void Component_Transform::Calculate_Angle_Axis()
{
	angle = 2 * acos(rotation.w);
	rotation_angle.x = rotation.x / sqrt(1 - rotation.w * rotation.w);
	rotation_angle.y = rotation.y / sqrt(1 - rotation.w * rotation.w);
	rotation_angle.z = rotation.z / sqrt(1 - rotation.w * rotation.w);

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