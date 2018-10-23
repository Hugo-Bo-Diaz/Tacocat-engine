#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	type = TRANSFORM;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Move(float3 newpos)
{
	pos += newpos;
}

void TransformComponent::Rotate(float3 newrot)
{
	quaternion.FromEulerXYZ(rot.x, rot.y, rot.z);

	/*Quat X, Y, Z;

	X.FromEulerXYZ(newrot.x, 0.0f, 0.0f);
	Y.FromEulerXYZ(0.0f, newrot.y, 0.0f);
	Z.FromEulerXYZ(0.0f, 0.0f, newrot.z);*/

	quaternion.RotateX(newrot.x);
	quaternion.RotateY(newrot.y);
	quaternion.RotateZ(newrot.z);

	rot = quaternion.ToEulerXYZ();
}

void TransformComponent::SetPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void TransformComponent::SetRot(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void TransformComponent::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

